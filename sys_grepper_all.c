#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ANSI escape codes for colors (optional)
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BOLD "\033[1m"
#define COLOR_NC    "\033[0m"
#define SEPARATOR "--------------------------------------------------"

#ifdef _WIN32
#define OS_TYPE "Windows"
#elif defined(__APPLE__)
#define OS_TYPE "macOS"
#else
#define OS_TYPE "Linux" // Or some other default

#endif

// Function prototypes
char* execute_command(const char* cmd);
void clear_screen();
void pause_me();
void system_information();
//void network_information();


// Helper function to execute a shell command and return the output
char* execute_command(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        return NULL; // Return NULL on error
    }

    char buffer[128];
    char* result = NULL;
    size_t result_size = 0;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t chunk_size = strlen(buffer);
        void* temp = realloc(result, result_size + chunk_size + 1); // +1 for null terminator

        if (!temp) {
            free(result); // Free allocated memory if realloc fails
            pclose(pipe);
            return NULL; // Return NULL on error
        }
        result = (char*)temp;
        strcpy(result + result_size, buffer); // Copy the chunk
        result_size += chunk_size;
    }

    pclose(pipe);

     //Trim trailing newline and other whitespace
    if (result) {
        size_t len = strlen(result);
        while (len > 0 && (result[len - 1] == '\n' || result[len - 1] == '\r' || result[len-1] == ' ' || result[len-1] == '\t')) {
            result[len - 1] = '\0';
            len--;
        }
    }

    return result;
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");  // Works on macOS and Linux
#endif
}

void pause_me() {
    printf("Press Enter to continue...");
    getchar(); // Waits for the user to press Enter
}

// Function to handle system information
void system_information()
{
    clear_screen();

    printf("%s%s%s\n", COLOR_GREEN, SEPARATOR, COLOR_NC);
    printf("%s%s SYSTEM INFORMATION %s\n", COLOR_YELLOW, COLOR_BOLD, COLOR_NC);
    printf("%s%s%s\n", COLOR_GREEN, SEPARATOR, COLOR_NC);

    printf("Operating System: %s%s%s\n", COLOR_GREEN, OS_TYPE, COLOR_NC);

    // Initialize all pointers to NULL
    char *win_ver = NULL;
    char *mac_ver = NULL, *mac_build = NULL, *mac_name = NULL, *mac_host = NULL, *mac_user = NULL, *mac_model = NULL;
    char *mac_serial = NULL, *mac_uuid = NULL, *mac_arch = NULL, *mac_kernel = NULL, *mac_uptime = NULL, *mac_load = NULL;
    char *mac_cpu = NULL, *mac_mem = NULL, *mac_swap = NULL, *mac_disk = NULL, *mac_temp = NULL, *mac_battery = NULL;
    char *mac_network = NULL, *mac_gateway = NULL;
    char *linux_ver = NULL, *linux_host = NULL, *linux_user = NULL;
    char *linux_model = NULL, *linux_arch = NULL, *linux_kernel = NULL, *linux_uptime = NULL, *linux_load = NULL;
    char *linux_cpu = NULL, *linux_mem = NULL, *linux_swap = NULL, *linux_disk = NULL, *linux_temp = NULL;
    char *linux_battery = NULL, *linux_network = NULL, *linux_gateway = NULL;

#ifdef _WIN32   
    win_ver = execute_command("ver");
    if (win_ver) {
        win_ver[strcspn(win_ver, "\r\n")] = 0;
        printf("Windows Version: %s%s%s\n", COLOR_GREEN, win_ver, COLOR_NC);
        free(win_ver);
    }

#elif defined(__APPLE__) // Use #elif for mutually exclusive conditions

    mac_ver = execute_command("sw_vers -productVersion");
    mac_build = execute_command("sw_vers -buildVersion");
    mac_name = execute_command("sw_vers -productName");
    mac_host = execute_command("scutil --get ComputerName");
    mac_user = execute_command("scutil --get UserName");
    mac_model = execute_command("sysctl -n hw.model");
    mac_serial = execute_command("system_profiler SPHardwareDataType | awk '/Serial/ {print $4}'");
    mac_uuid = execute_command("system_profiler SPHardwareDataType | awk '/Hardware UUID/ {print $3}'");
    mac_arch = execute_command("uname -m");
    mac_kernel = execute_command("uname -r");
    mac_uptime = execute_command("uptime");
    mac_load = execute_command("sysctl -n vm.loadavg");
    mac_cpu = execute_command("sysctl -n hw.ncpu");
    mac_mem = execute_command("sysctl -n hw.memsize");
    mac_swap = execute_command("sysctl -n vm.swapusage");
    mac_disk = execute_command("df -h / | awk '/\\// {print $4}'"); // Correct escaping
    mac_temp = execute_command("sysctl -n machdep.xcpm.cpu_thermal_level");
    mac_battery = execute_command("pmset -g batt | grep -o '[0-9]*%'");
    mac_network = execute_command("ifconfig en0 | grep 'inet ' | awk '{print $2}'");
    mac_gateway = execute_command("netstat -nr | grep 'default' | awk '{print $2}'");


    if (mac_ver)    { printf("[+] macOS Version:        %s%s%s\n", COLOR_GREEN, mac_ver,    COLOR_NC); free(mac_ver);    }
    if (mac_build)  { printf("[+] Build Version:        %s%s%s\n", COLOR_GREEN, mac_build,  COLOR_NC); free(mac_build);  }
    if (mac_name)   { printf("[+] Product Name:         %s%s%s\n", COLOR_GREEN, mac_name,   COLOR_NC); free(mac_name);   }
    if (mac_host)   { printf("[+] Computer Name:        %s%s%s\n", COLOR_GREEN, mac_host,   COLOR_NC); free(mac_host);   }
    if (mac_user)   { printf("[+] User Name:            %s%s%s\n", COLOR_GREEN, mac_user,   COLOR_NC); free(mac_user);   }
    if (mac_model)  { printf("[+] Model:                %s%s%s\n", COLOR_GREEN, mac_model,  COLOR_NC); free(mac_model);  }
    if (mac_serial) { printf("[+] Serial Number:        %s%s%s\n", COLOR_GREEN, mac_serial, COLOR_NC); free(mac_serial); }
    if (mac_uuid)   { printf("[+] Hardware UUID:        %s%s%s\n", COLOR_GREEN, mac_uuid,   COLOR_NC); free(mac_uuid);   }
    if (mac_arch)   { printf("[+] Architecture:         %s%s%s\n", COLOR_GREEN, mac_arch,   COLOR_NC); free(mac_arch);   }
    if (mac_kernel) { printf("[+] Kernel Version:       %s%s%s\n", COLOR_GREEN, mac_kernel, COLOR_NC); free(mac_kernel); }
    if (mac_uptime) { printf("[+] Uptime:               %s%s%s\n", COLOR_GREEN, mac_uptime, COLOR_NC); free(mac_uptime); }
    if (mac_load)   { printf("[+] Load Average:         %s%s%s\n", COLOR_GREEN, mac_load,   COLOR_NC); free(mac_load);   }
    if (mac_cpu)    { printf("[+] CPU Count:            %s%s%s\n", COLOR_GREEN, mac_cpu,    COLOR_NC); free(mac_cpu);    }
    if (mac_mem)    { printf("[+] Memory Size:          %s%s%s bytes\n", COLOR_GREEN, mac_mem,    COLOR_NC); free(mac_mem);    }
    if (mac_swap)   { printf("[+] Swap Usage:           %s%s%s\n", COLOR_GREEN, mac_swap,   COLOR_NC); free(mac_swap);   }
    if (mac_disk)   { printf("[+] Disk Space Available: %s%s%s\n", COLOR_GREEN, mac_disk,   COLOR_NC); free(mac_disk);   }
    if (mac_temp)   { printf("[+] CPU Thermal Level:    %s%s%s\n", COLOR_GREEN, mac_temp,   COLOR_NC); free(mac_temp);   }
    if (mac_battery){ printf("[+] Battery Percentage:   %s%s%s\n", COLOR_GREEN, mac_battery,COLOR_NC); free(mac_battery);}
    if (mac_network){ printf("[+] Network IP Address:   %s%s%s\n", COLOR_GREEN, mac_network,COLOR_NC); free(mac_network);}
    if (mac_gateway){ printf("[+] Gateway IP Address:   %s%s%s\n", COLOR_GREEN, mac_gateway,COLOR_NC); free(mac_gateway);}

#else
    // Linux (or some other POSIX-compliant system)
    linux_ver = execute_command("cat /etc/issue | head -n 1");
    linux_host = execute_command("hostname");
    linux_user = execute_command("whoami");
    linux_model = execute_command("uname -m"); //  This is usually architecture, not model.
    linux_arch = execute_command("uname -m");
    linux_kernel = execute_command("uname -r");
    linux_uptime = execute_command("uptime -p");
    linux_load = execute_command("uptime | awk '{print $10 $11 $12}'"); // Corrected
    linux_cpu = execute_command("nproc");
    linux_mem = execute_command("free -h | awk '/Mem:/ {print $2}'");
    linux_swap = execute_command("free -h | awk '/Swap:/ {print $2}'");
    linux_disk = execute_command("df -h / | awk '/\\// {print $4}'"); // Corrected escaping
    linux_temp = execute_command("sensors | grep 'Package id 0' | awk '{print $4}'"); // Might need adjustment
    linux_battery = execute_command("acpi -b | grep 'Battery 0' | awk '{print $4}'"); // Might need adjustment
    linux_network = execute_command("hostname -I | awk '{print $1}'");
    linux_gateway = execute_command("ip route | grep 'default' | awk '{print $3}'");

    if (linux_ver) {
        printf("[+] Linux Distribution:   %s%s%s\n", COLOR_GREEN, linux_ver, COLOR_NC);
        free(linux_ver);
    }
    
    if (linux_host) {
        printf("[+] Hostname:             %s%s%s\n", COLOR_GREEN, linux_host, COLOR_NC);
        free(linux_host);
    }
    
    if (linux_user) {
        printf("[+] User Name:            %s%s%s\n", COLOR_GREEN, linux_user, COLOR_NC);
        free(linux_user);
    }
    
    if (linux_model) {
        printf("[+] Model:                %s%s%s\n", COLOR_GREEN, linux_model, COLOR_NC);
        free(linux_model);
    }
    
    if (linux_arch) {
        printf("[+] Architecture:         %s%s%s\n", COLOR_GREEN, linux_arch, COLOR_NC);
        free(linux_arch);
    }
    
    if (linux_kernel) {
        linux_kernel[strcspn(linux_kernel, "\r\n")] = 0;
        printf("[+] Kernel Version:       %s%s%s\n", COLOR_GREEN, linux_kernel, COLOR_NC);
        free(linux_kernel);
    }
    
    if (linux_uptime) {
        printf("[+] Uptime:               %s%s%s\n", COLOR_GREEN, linux_uptime, COLOR_NC);
        free(linux_uptime);
    }
    
    if (linux_load) {
        printf("[+] Load Average:         %s%s%s\n", COLOR_GREEN, linux_load, COLOR_NC);
        free(linux_load);
    }
    
    if (linux_cpu) {
        printf("[+] CPU Count:            %s%s%s\n", COLOR_GREEN, linux_cpu, COLOR_NC);
        free(linux_cpu);
    }
    
    if (linux_mem) {
        printf("[+] Memory Size:          %s%s%s\n", COLOR_GREEN, linux_mem, COLOR_NC);
        free(linux_mem);
    }
    
    if (linux_swap) {
        printf("[+] Swap Usage:           %s%s%s\n", COLOR_GREEN, linux_swap, COLOR_NC);
        free(linux_swap);
    }
    
    if (linux_disk) {
        printf("[+] Disk Space Available: %s%s%s\n", COLOR_GREEN, linux_disk, COLOR_NC);
        free(linux_disk);
    }
    
    if (linux_temp) {
        printf("[+] CPU Thermal Level:    %s%s%s\n", COLOR_GREEN, linux_temp, COLOR_NC);
        free(linux_temp);
    }
    
    if (linux_battery) {
        printf("[+] Battery Percentage:   %s%s%s\n", COLOR_GREEN, linux_battery, COLOR_NC);
        free(linux_battery);
    }
    
    if (linux_network) {
        printf("[+] Network IP Address:   %s%s%s\n", COLOR_GREEN, linux_network, COLOR_NC);
        free(linux_network);
    }
    
    if (linux_gateway) {
        printf("[+] Gateway IP Address:   %s%s%s\n", COLOR_GREEN, linux_gateway, COLOR_NC);
        free(linux_gateway);
    }
#endif
}
