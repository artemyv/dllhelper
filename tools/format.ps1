param(
    [Parameter(Mandatory=$false)]
    [string]$ClangFormatPath = 'D:\progs\clang-format-19\clang-format.exe'
)

$paths = @('include','src','tests','examples')
$exts = @('.h','.hpp','.hh','.hxx','.c','.cc','.cxx','.cpp','.inl')

$files = @()
foreach ($p in $paths) {
    if (Test-Path $p) {
        $files += Get-ChildItem -Path $p -Recurse -File | Where-Object { $exts -contains $_.Extension }
    }
}

$count = $files.Count
Write-Host "Formatting $count files..."

foreach ($f in $files) {
    & $ClangFormatPath -i -style=file $f.FullName
}

Write-Host "Formatted $count files."

if (Test-Path '.git') {
    $changed = (git status --porcelain | Measure-Object).Count
    Write-Host "Changed files: $changed"
}
