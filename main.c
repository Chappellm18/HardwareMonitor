#include <stdio.h>
#include <windows.h>
#include <pdh.h>
#include <gtk/gtk.h>

#pragma comment(lib, "pdh.lib")

// Function to get the current CPU usage
double CpuUsage() {
    static PDH_HQUERY query;
    static PDH_HCOUNTER total;
    static BOOL init = FALSE;
    PDH_FMT_COUNTERVALUE counter;

    // If not initialized, start it up
    if (!init) {
        PdhOpenQuery(NULL, 0, &query);
        PdhAddCounter(query, TEXT("\\Processor(_Total)\\% Processor Time"), 0, &total);
        PdhCollectQueryData(query);
        init = TRUE;
        Sleep(1000); // Wait for a valid reading
    }

    PdhCollectQueryData(query);
    PdhGetFormattedCounterValue(total, PDH_FMT_DOUBLE, NULL, &counter);
    return counter.doubleValue;
}

// Function that runs for a given time
void monitorCPUUsage(int duration) {
    printf("Monitoring CPU usage for %d seconds...\n", duration);
    for (int i = 0; i < duration; i++) {
        double cpuUsage = CpuUsage();
        printf("CPU Usage: %.2f%%\n", cpuUsage);
        Sleep(1000); // Wait 1 second
    }
}

// Function to display GUI data
void displayData(GtkWidget* widget, gpointer data) {
    g_print("Data here\n");
}

// Function to destroy GUI
void destroy(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

// GUI Initialization
void initGui(int argc, char *argv[]) {
    GtkWidget* window;
    GtkWidget* button;
    GtkWidget* box;  // GTK+3 requires a container like GtkBox

    gtk_init(&argc, &argv);

    // Create window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    // Create a vertical box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create button
    button = gtk_button_new_with_label("Click Me!");
    g_signal_connect(button, "clicked", G_CALLBACK(displayData), NULL);

    // Add button to the box
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    gtk_main();
}

// Entry Point
int main(int argc, char *argv[]) {
    initGui(argc, argv);
    return 0;
}

