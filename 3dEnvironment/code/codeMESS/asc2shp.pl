#!/usr/bin/perl

#
# Converts the 3dstudio ASC 3d geometry file format to a simple points and
# polygons format for easy parsing by the graphics engine
#

$faces=0;
while(<>){

    # Finds an object
    if(/^Named\sobject:\s"([^\s]*)"/)
    {
	print "Object ". $1." ";
    }

    # Gets every point in that object
    if(/^Vertex \d+:\s*X:\s*([\d\.-]+)\s*Y:\s*([\d\.-]+)\s*Z:\s*([\d\.-]+)/) 
    {
	$faces=0;
$s=1.15;
$x1=$1 + 0.1;
$x2=$2 + 0.1;
$x3=$3 + 1.1;
	$x1*=$s;
	$x2*=$s;
	$x3*=$s;
	print $x1." ".$x2." ".$x3." \n";
    }

    # Finally gets a list of triangles
    if(/^Face \d+:\s*A:\s*([\d\.-]+)\s*B:\s*([\d\.-]+)\s*C:\s*([\d\.-]+)\s*AB:(\d)\s*BC:(\d)\s*CA:(\d)/)
    {
	if($faces==0){
	    $faces=1;
	    print "Polygons ";
	}
	print $1." ".$2." ".$3." ".$4." ".$5." ".$6." \n";
    }
}

