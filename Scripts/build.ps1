# Windows PowerShell build script

# Step 0: Get the current script's directory
$dir = $PSScriptRoot

# Step 1: Configure the project with CMake
Write-Host "Configuring the project with CMake..."
cmake -S $dir/../ -B $dir/../Builds

# Check if the configuration step was successful
if ($LastExitCode -ne 0) {
    Write-Host "CMake configuration failed. Exiting."
    exit $LastExitCode
}

# Step 2: Build the project with CMake
Write-Host "Building the project with CMake..."
cmake --build $dir/../Builds

# Check if the build step was successful
if ($LastExitCode -ne 0) {
    Write-Host "CMake build failed. Exiting."
    exit $LastExitCode
}

Write-Host "Build completed successfully."
