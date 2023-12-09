# parameter for day number 1 to 25
param (
    [Parameter(Mandatory=$true)]
    [int]$daynum
)

# fail on any error
$ErrorActionPreference = "Stop"

# convert to string day01..day25
$day = "day" + $daynum.ToString("D2")

$path = ($PSScriptRoot + "/" + $day);
$template_path = ($PSScriptRoot + "/template");
# if folder exists then fail
if (Test-Path $path) {
    throw "Folder $day already exists"
}
# create folder
New-Item -ItemType Directory -Force -Path $path;

Push-Location $template_path
# If $template_path/tmp does not exist then create it
if (-not (Test-Path tmp)) {
    New-Item -ItemType Directory -Force -Path tmp;
}
Push-Location tmp
# run cmake in $template_path folder with option AOC_DAY=$day
# e.g. cmake -DAOC_DAY=day01 -S template -B day01
cmake .. -DAOC_DAY="$day" --fresh
Pop-Location
