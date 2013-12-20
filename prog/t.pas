const a = 1,b = 'a', c = -122;
var x,y:integer;
	z:char;

function gcd(a,b:integer):integer;
var t:integer;
	function mod(a,b:integer):integer;
	begin
		mod := a - a / b * b 
	end;
begin
	repeat
		begin
		t := b;
		b := mod(a,b);
		a := t
		end
	until b = 0;
	gcd := a
end;

begin
	write("shabi\n");
	read(x);
	read(y);
	write(gcd(x,y))
end.
