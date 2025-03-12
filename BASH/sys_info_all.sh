#!/bin/bash

# Function to gather and display system information
function _broadcastInfo() {
    output_file="system_info.txt"

    # Clear the file before starting
    > "$output_file"
    
    # Add a header to the output file
    echo "==================== SYSTEM INFORMATION ====================" > "$output_file"
    echo "Generated on: $(date)" >> "$output_file"
    echo "------------------------------------------------------------" >> "$output_file"
    
    # Determine the platform
    platform=$(uname)
    
    # Function to get broadcast information
    get_broadcast_info() {
        if [[ "$platform" == "Linux" ]]; then
            if command -v ifconfig > /dev/null; then
                ifconfig | grep broadcast
            elif command -v ip > /dev/null; then
                ip addr | grep broadcast
            else
                echo "Command not found"
            fi
        elif [[ "$platform" == "Darwin" ]]; then
            ifconfig | grep broadcast
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            ipconfig | findstr /i "broadcast"
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get IP (inet) information
    get_ip_info() {
        if [[ "$platform" == "Linux" ]]; then
            if command -v ifconfig > /dev/null; then
                ifconfig | grep inet
            elif command -v ip > /dev/null; then
                ip -4 addr show
            else
                echo "Command not found"
            fi
        elif [[ "$platform" == "Darwin" ]]; then
            ifconfig | grep inet
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            ipconfig
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get MAC address information
    get_mac_info() {
        if [[ "$platform" == "Linux" ]]; then
            if command -v ifconfig > /dev/null; then
                ifconfig | grep ether
            elif command -v ip > /dev/null; then
                ip link show | grep ether
            else
                echo "Command not found"
            fi
        elif [[ "$platform" == "Darwin" ]]; then
            ifconfig | grep ether
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            ipconfig /all | findstr /i "physical"
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get radio interface name
    get_radio_name() {
        if [[ "$platform" == "Linux" ]]; then
            if command -v iw > /dev/null; then
                iw dev | awk '$1=="Interface"{print $2}'
            else
                echo "Command not found"
            fi
        elif [[ "$platform" == "Darwin" || "$platform" == "MINGW64_NT"* ]]; then
            echo "Not applicable"
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get USB devices
    get_usb_info() {
        if [[ "$platform" == "Linux" ]]; then
            if command -v lsusb > /dev/null; then
                lsusb
            else
                echo "Command not found"
            fi
        elif [[ "$platform" == "Darwin" ]]; then
            system_profiler SPUSBDataType
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            echo "Not applicable"
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get CPU information
    get_cpu_info() {
        if [[ "$platform" == "Linux" ]]; then
            lscpu
        elif [[ "$platform" == "Darwin" ]]; then
            sysctl -n machdep.cpu.brand_string
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            wmic cpu get name
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get memory information
    get_mem_info() {
        if [[ "$platform" == "Linux" ]]; then
            free -h
        elif [[ "$platform" == "Darwin" ]]; then
            vm_stat
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            systeminfo | findstr /C:"Total Physical Memory"
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get disk information
    get_disk_info() {
        if [[ "$platform" == "Linux" ]]; then
            df -h
        elif [[ "$platform" == "Darwin" ]]; then
            df -h
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            wmic logicaldisk get size,freespace,caption
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get network information
    get_net_info() {
        if [[ "$platform" == "Linux" ]]; then
            netstat -tuln
        elif [[ "$platform" == "Darwin" ]]; then
            netstat -an
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            netstat -an
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get OS details
    get_os_info() {
        if [[ "$platform" == "Linux" ]]; then
            cat /etc/os-release
        elif [[ "$platform" == "Darwin" ]]; then
            sw_vers
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            systeminfo | findstr /B /C:"OS Name" /C:"OS Version"
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get running processes
    get_proc_info() {
        if [[ "$platform" == "Linux" ]]; then
            ps aux --sort=-%mem | head -n 10
        elif [[ "$platform" == "Darwin" ]]; then
            top -l 1 -o mem
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            tasklist | sort /R /+64 | findstr /V "System Idle Process"
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get installed packages
    get_pkg_info() {
        if [[ "$platform" == "Linux" ]]; then
            dpkg -l
        elif [[ "$platform" == "Darwin" ]]; then
            brew list --versions
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            wmic product get name,version
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get environment variables
    get_env_info() {
        if [[ "$platform" == "Linux" || "$platform" == "Darwin" ]]; then
            printenv
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            set
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get file system information
    get_fs_info() {
        if [[ "$platform" == "Linux" ]]; then
            df -T
        elif [[ "$platform" == "Darwin" ]]; then
            df -T hfs
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            wmic logicaldisk get filesystem
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get hardware information
    get_hw_info() {
        if [[ "$platform" == "Linux" ]]; then
            lshw -short
        elif [[ "$platform" == "Darwin" ]]; then
            system_profiler SPHardwareDataType
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            wmic computersystem get model,manufacturer
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get temperature sensors
    get_temp_info() {
        if [[ "$platform" == "Linux" ]]; then
            sensors
        elif [[ "$platform" == "Darwin" ]]; then
            osx-cpu-temp
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            echo "Not applicable"
        else
            echo "Unsupported platform"
        fi
    }

    # Function to get battery information
    get_battery_info() {
        if [[ "$platform" == "Linux" ]]; then
            upower -i $(upower -e | grep BAT)
        elif [[ "$platform" == "Darwin" ]]; then
            pmset -g batt
        elif [[ "$platform" == "MINGW64_NT"* ]]; then
            powercfg /batteryreport
        else
            echo "Unsupported platform"
        fi
    }

    # Display and store broadcast information
    echo -e "\n[*] Getting Broadcast Information..."
    _broadcast=$(get_broadcast_info)
    echo -e "Broadcast Info:\n$_broadcast"
    echo -e "\n[*] Broadcast Info:\n$_broadcast" >> "$output_file"

    # Display and store IP information
    echo -e "\n[*] Getting IP (inet) Information..."
    _inet=$(get_ip_info)
    echo -e "IP Info:\n
