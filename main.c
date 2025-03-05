#include <stdio.h>
#include <windows.h>
#include <pdh.h>

#pragma commnet(lib, "pdh.lib")

// Function to get the current cpu usage
double CpuUsage() {
	static PDH_HQUERY query;
	static PDH_HCOUNTER total;
	static BOOL init = FALSE;
	PDH_FMT_COUNTERVALUE counter;

	// If not initialized start it up
	if(!init) {
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

int main() {
    int duration;

    printf("Enter duration (in seconds): ");
    scanf("%d", &duration);

    monitorCPUUsage(duration);

    printf("Monitoring complete.\n");
    return 0;
}
