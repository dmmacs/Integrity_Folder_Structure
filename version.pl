

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

if ($numArgs < 3)
{	
	print "usage: perl.exe version.pl <project .pro file> <version.pro> <build sub folder i.e. release>\n";
	exit (1);
}

my $target;
my $outData;

#Parse the project .pro file to find the target exe
open (IN, "<$ARGV[$PRJ_PRO_INDEX]");
while(<IN>)
{
	chomp();
#	print $_ . "\n";
	
	if (/TARGET/)
	{
		($target) = ($_ =~ /\s*?TARGET\s*?=\s*(.*)/);
		s/\s*?TARGET\s*?=\s*//;
		#print $_ . "\n";
		$target = $target . ".exe";
		#print $target . "\n";
	
	}
}
close(IN);


#Get the Exe Version Information
my $cmd = "cscript //nologo VersionInfo.vbs $ARGV[2]\\$target";
my $exeVersion = `$cmd`;
chomp($exeVersion);
($exeMajor,$exeMinor,$exePatch,$exeBuild) = ($exeVersion =~ /([0-9]*)\.([0-9]*)\.([0-9]*)\.(.*)/);

#Parse the version.pro to possibly up the version
open (IN, "<$ARGV[$VER_PRO_INDEX]");
while (<IN>)
{
	chomp();
	if (/VERSION/)
	{
		($buildVersion) = ($_ =~ /\s*?VERSION\s*?=\s*(.*)/);
		($major,$minor,$patch,$build) = ($buildVersion =~ /([0-9]*)\.([0-9]*)\.([0-9]*)\.(.*)/);
		if ($buildVersion eq $exeVersion)
		{
			if (lc($ARGV[$BLD_TYPE_INDEX]) eq "release")
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

#print $buildVersion . "\n";

#print $major . "," . $minor . "," . $patch. "," . $build . "\n";
#print $exeMajor . "," . $exeMinor . "," . $exePatch. "," . $exeBuild . "\n";
#print $outData . "\n";

if ($versionChanged == 1)
{
	open (OUT, ">$ARGV[$VER_PRO_INDEX]");
	print OUT $outData;
	close (OUT);
}
exit (0);
