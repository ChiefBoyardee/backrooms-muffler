$input = [Console]::In.ReadToEnd() | ConvertFrom-Json

$context = @"
Backrooms Muffler — JUCE 8 VST3 liminal muffle effect.
Build: cmake --preset vs2022; cmake --build --preset vs2022-release
MCP: context7 (/websites/juce_master) for JUCE; brave-search for web; thoughtbox for deep reasoning.
Read docs/DSP.md before tuning, docs/BUILD.md for CI/Ableton. See CHANGELOG.md for version history.
"@

@{ additional_context = $context } | ConvertTo-Json -Compress
