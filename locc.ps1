$directoryPath = "./Source"
$totalLoc = 0
$files = Get-ChildItem -Path $directoryPath -File -Include *.h,*.cpp -Recurse

foreach ($file in $files) {
    $loc = (Get-Content $file.FullName | Measure-Object -Line).Lines
    Write-Host "File: $($file.FullName), LOC: $loc"
    $totalLoc += $loc
}

Write-Host "Total LOC in folder: $totalLoc"