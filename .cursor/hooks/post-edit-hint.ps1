$input = [Console]::In.ReadToEnd() | ConvertFrom-Json
$path = $input.file_path

if ($path -match 'source[\\/]dsp') {
    @{
        additional_context = "DSP file edited. Run: cmake --build build --config Release --target BackroomsMufflerTests; then ctest --test-dir build -C Release. Rebuild VST3 before Ableton testing."
    } | ConvertTo-Json -Compress
    exit 0
}

exit 0
