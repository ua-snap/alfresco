#!/usr/bin/perl -w
#
use strict;

use warnings;

my %variableHash = ();

open FIF, $ARGV[0] or die $!;

while (my $line = <FIF>){
	chomp($line);
	$line =~ s/;.*//;
	my @linevals = $line =~ /(.*)\s*\=\s*(.*)\s*/;
	if ($linevals[0] && $linevals[0] ne ""){
		$linevals[0] =~ s/\s*//g;
		my $key = $linevals[0];
		$linevals[1] =~ s/\s*//g;
		my $value = $linevals[1];
		$variableHash{$key} = $value;
		#print $linevals[0]." = ";
		#print $linevals[1]."\n";
	}
}
close(FIF);

# Perform checks as to whether or not the variable is declared and/or defined
sub definitionChecks {
	if (exists $variableHash{$_[0]}){
		if (defined $variableHash{$_[0]}){
			return 1;
		} else {
			print "NOT SET: ".$_[0]."\n";
			return 0;
		}
	} else {
		print "UNDEFINED: ".$_[0]."\n";
		return 0;
	}
}

if (definitionChecks("ClientInputBasePath")){
	my ($path) = $variableHash{"ClientInputBasePath"} =~ /"([^"]*)"/;
	if (-d $path){

	} else {
		print "PATH DOES NOT EXIST: ".$variableHash{"ClientInputBasePath"}."\n"; 
	}
}

definitionChecks("ClientFifPath");
definitionChecks("FirstYear");
definitionChecks("GraminoidTundra");
definitionChecks("LastYear");
definitionChecks("MaxReps");
definitionChecks("NoVeg");
definitionChecks("RandSeed");
definitionChecks("ShrubTundra");
definitionChecks("Tundra");
definitionChecks("WetlandTundra");
definitionChecks("XOffset");
definitionChecks("XSize");
definitionChecks("XULCorner");
definitionChecks("YOffset");
definitionChecks("YSize");
definitionChecks("YULCorner");
