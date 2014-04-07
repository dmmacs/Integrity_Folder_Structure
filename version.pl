

my $fname = shift;


open (IN, "<$fname");

my $major;
my $minor;
my $build;
my $outData;

while(<IN>)
{
	chomp();
	if (/static const int MAJOR/)
	{
		
		($major) = ($_ =~ /.*?=\s*([0-9]*);/);
		s/[0-9]*;//;
		#need to figure out criteria on how to sync with Git Tag and update data or use command line???
		#$major++;
		$outData = $outData . $_ . $major . ";" ."\n";
		#s/.*?=\s*//;
		#s/;//;
		#$major = $_;
		
#		print $_ . "\n";
	}
	elsif (/static const int MINOR/)
	{
		($minor) = ($_ =~ /.*?=\s*([0-9]*);/);
		s/[0-9]*;//;
		#need to figure out criteria on how to sync with Git Tag and update data or use command line???
		#$minor++;
		$outData = $outData . $_ . $minor . ";" ."\n";
	}
	elsif (/static const int BUILD/)
	{
		($build) = ($_ =~ /.*?=\s*([0-9]*);/);
		s/[0-9]*;//;
		#need to figure out criteria on how to sync with Git Tag and update data or use command line???
		$build++;
		$outData = $outData . $_ . $build . ";" ."\n";
	}
	else
	{
		$outData = $outData . $_ . "\n";
	}
	
}

close (IN);

open (OUT, ">$fname");

print OUT $outData;

exit (0);