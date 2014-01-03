var
b,a:array[10]of integer;
i,n,sum:integer;
procedure qs(var sum:integer;l,r:integer);
var
i,j,t,x:integer;
procedure swap(var x,y:integer);
var
tmp:integer;
begin
     tmp:=x;x:=y;y:=tmp;
end;
begin
sum:=5;
x:=a[(r+l)/ 2];
i:=l;j:=r;
if i<=j then begin
  repeat begin
     if a[i]<x then begin
      repeat
		begin
        i:=i+1
		end
      until a[i]>=x; 
     end;
     if a[j]>x then begin
      repeat
		begin
        j:=j-1
		end
      until a[j]<=x;
     end;
     if i<=j then begin
       swap(a[i],a[j]);
       i:=i+1;
       j:=j-1;
     end;
  end until i>j;
end;
if i<r then qs(sum,i,r);
if j>l then qs(sum,l,j);
end;
begin
    n:=9;
    a[1]:=2;a[2]:=3;a[3]:=1;a[4]:=8;a[5]:=6;a[6]:=5;a[7]:=0;a[8]:=9;a[9]:=4;
    qs(sum,1,n);
    for i:=1 to n do b[i]:=1;
    for i:=1 to n-1 do begin
      if a[i]=a[i+1] then begin
        b[i+1]:=b[i]+b[i+1];
        b[i]:=0;
      end;
    end;
    for i:=1 to n do write(a[i]);
    write(sum);
end.
