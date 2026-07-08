$input = [Console]::In.ReadToEnd() | ConvertFrom-Json
$command = $input.command

if ($command -match 'git clean|git reset --hard') {
    @{
        permission = "ask"
        user_message = "This command may delete uncommitted work. Please review before continuing."
        agent_message = "Hook flagged a potentially destructive git command."
    } | ConvertTo-Json -Compress
    exit 0
}

@{ permission = "allow" } | ConvertTo-Json -Compress
exit 0
