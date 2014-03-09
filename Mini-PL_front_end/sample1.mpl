var nimi : string;
print "Anna nimesi!";
read nimi;
var x : int;
for x in 0..5 do
  //print x;
  print "Moi " + nimi;
end for;

var nTimes : int := 0;
// lets test a single line comment!"#¤%&/()=?@£${[]}´`¨^~'*,;.:-_//
print "How many times?";
read nTimes;
x := 0;
for x in 0..nTimes-1 do
  print x;
  print " : Hello, World!\n";
end for;
assert (x = nTimes);