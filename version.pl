
#Command Line Argument data
$debug = 0;
$Project_File = "";
$Version_File = "";
$Build_Type = "";


#Constants
my $PRJ_PRO_INDEX = 0;
my $VER_PRO_INDEX = 1;
my $BLD_TYPE_INDEX = 2;

#Variables
my $major;
my $minor;
my $patch; 
my $build;

my $exeMajor;
my $exeMinor;
my $exePatch; 
my $exeBuild;

my $buildVersion;

my $versionChanged = 0;

$numArgs = $#ARGV + 1;

processArgs(@ARGV);

#if ($numArgs < 3)
#{	
#	print "usage: perl.exe version.pl <project .pro file> <version.pro> <build sub folder i.e. release>\n";
#	exit (1);
#}

my $target;
my $outData;

#Parse the project .pro file to find the target exe
open (IN, "<$Project_File");
while(<IN>)
{
	chomp();
#	print $_ . "\n";
	
	if (/TARGET/)
	{
		($target) = ($_ =~ /\s*?TARGET\s*?=\s*(.*)/);
		s/\s*?TARGET\s*?=\s*//;
		$target = $target . ".exe";
	}
}
close(IN);

if ($debug == 1)
{
	print "Target=" . $target . "\n";
}

#Get the Exe Version Information
my $cmd = "cscript //nologo VersionInfo.vbs $Build_Type\\$target";
if ($debug == 1)
{
	print "vbs command: $cmd\n";
}
my $exeVersion = `$cmd`;
chomp($exeVersion);
if ($debug == 1)
{
	print "vbs output: $exeVersion\n";
}


($exeMajor,$exeMinor,$exePatch,$exeBuild) = ($exeVersion =~ /([0-9]*)\.([0-9]*)\.([0-9]*)\.(.*)/);

#Parse the version.pro to possibly up the version
open (IN, "<$Version_File");
while (<IN>)
{
	chomp();
	if (/VERSION/)
	{
		($buildVersion) = ($_ =~ /\s*?VERSION\s*?=\s*(.*)/);
		($major,$minor,$patch,$build) = ($buildVersion =~ /([0-9]*)\.([0-9]*)\.([0-9]*)\.(.*)/);
		if ($buildVersion eq $exeVersion)
		{
			if (lc($Build_Type) eq "release")
			{
				$patch++;
				$build = 0;
				$versionChanged = 1;
			}
			else
			{
				$build++;
				$versionChanged = 1;
			}
		}
		$outData = $outData . "VERSION = " . "$major.$minor.$patch.$build\n";
	}
	else
	{
		$outData = $outData . $_ . "\n";
	}
	
}


close (IN);
if ($debug == 1)
{
	print "Build Version: $buildVersion\n";
	print "Version Changed: $versionChanged\n";
	print "Output Data: \n";
	print $outData;

}

if ($versionChanged == 1)
{
	open (OUT, ">$Version_File");
	print OUT $outData;
	close (OUT);
}
exit (0);

sub processArgs()
{
	local @ARGV = @_;

	$numArgs = $#ARGV + 1;
	#print $numArgs . "\n";
	foreach $argnum (0 .. $#ARGV) 
	{
		if ($ARGV[$argnum] =~ /-d/)
		{
			$debug = 1;
		}
		elsif ($ARGV[$argnum] =~ /-p/)
		{
			($Project_File) = ($ARGV[$argnum] =~ /-p(.*)/);
		}
		elsif ($ARGV[$argnum] =~ /-v/)
		{
			($Version_File) = ($ARGV[$argnum] =~ /-v(.*)/);
		}
		elsif ($ARGV[$argnum] =~ /-b/)
		{
			($Build_Type) = ($ARGV[$argnum] =~ /-b(.*)/);
		}

	}

	if ($debug == 1)
	{
		foreach $argnum (0 .. $#ARGV) 
		{
			print "$ARGV[$argnum]\n";
		}
		print "Project File: $Project_File\n";
		print "Version File: $Version_File\n";
		print "Build Type: $Build_Type\n";
	}
	
	if ($numArgs < 3)
	{	
		print "usage: perl.exe version.pl <options>\n";
		print "Options:\n";
		print "-d\t\t\tTurn on debug output\n";
		print "-p<filename>\t\tProject file to find the target exe\n";
		print "-v<filename>\t\tFile to pull the current version information from>\n";
		print "-b<Build type>\t\tSetting to release will update the patch number, Debug will update the build number by default\n";
				
		exit (1);
	}
	
}