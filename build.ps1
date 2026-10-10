param([ValidateSet('all', 'check', 'pdf', 'format', 'pack')][string]$Action = 'all')
$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot
$blPython = if ($env:PYTHON) { $env:PYTHON } else {
    $blBundled = Join-Path $env:USERPROFILE '.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe'
    if (Test-Path -LiteralPath $blBundled) { $blBundled } else { 'python' }
}
function Invoke-BlueLagoon([string[]]$Arguments) {
    & $blPython @Arguments
    if ($LASTEXITCODE -ne 0) { throw "Build command failed: $Arguments" }
}
if ($Action -in @('all', 'check')) { Invoke-BlueLagoon @('scripts/verify.py') }
if ($Action -in @('all', 'pdf')) { Invoke-BlueLagoon @('scripts/build.py') }
if ($Action -eq 'format') {
    Invoke-BlueLagoon @('scripts/sync_metadata.py')
    Invoke-BlueLagoon @('scripts/style.py')
}
if ($Action -eq 'pack') { Invoke-BlueLagoon @('scripts/pack.py') }
