const a='a', b='b', c='c';
var m,count:integer;
	ch:char;

procedure move(getone,putone:char);
begin 
write(getone);
write("to",putone);
write(ch);
end;
procedure hanoi(n:integer;one,two,three:char);
begin
count:=count+1;
if n=1 then
move(one,three) 
else
	begin
	hanoi(n-1,one,three,two);
	move(one,three);
	hanoi(n-1,two,one,three)
	end
end;
begin
count:= 0;
read(m);
ch:= 10;
write("the step to moving disks");
write(ch);
hanoi(m,a,b,c);
write(count)
end.
