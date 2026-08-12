function Unzip
{
	Param([string]$zipfile)
	$outpath = Split-Path -parent $zipfile
		
	Push-Location -Path $outpath
	cmake.exe -E tar x $zipfile
	Pop-Location

	$outname = $outpath + "\" + (Get-Item $zipfile).BaseName
	(Get-Item $outname).LastWriteTime = Get-Date
}

if($args.length -gt 2)
{
	Echo "Usage: [file to unzip (without .zip)] ([do not create timestamp])"
	Exit
}

$dir = $args[0]
$file = $dir + '.zip'

if(-not (Test-Path $file))
{
	Echo "Zip file $file does not exist"
}
Unzip $file