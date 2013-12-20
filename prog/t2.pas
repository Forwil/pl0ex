var x,y:integer;
function gcd(a,b:integer):integer;
begin
	if b=0 then
		gcd :=a
	else
		gcd := gcd(b,a - a/b*b)
end;

begin
	write("shabi\n");
	read(x);
	read(y);
	write(gcd(x,y))
end.
