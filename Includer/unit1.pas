unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls;

type

  { TForm1 }

  TForm1 = class(TForm)
    IncludeButton: TButton;
    Memo1: TMemo;
    OpenDialog1: TOpenDialog;
    procedure IncludeButtonClick(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }


function FindInclude(var TxtList:TStringList): integer;
var
   i:integer;
begin
  Result:=-1;
  for i:=0 to TxtList.Count-1 do
    begin
      if ( Pos('#include',TxtList.Strings[i]) >=1) then
        begin
          Result:=i; break;
        end;
    end;
 end;

function ExtractString( Line:string):string;
begin
 Line:= Copy(Line,Pos('"',Line)+1,1024);
 Line:= Copy(Line,1,Pos('"',Line)-1);
 result:=Line;
end;

procedure TForm1.IncludeButtonClick(Sender: TObject);
var Orig,Header:TStringList;
    p1,p2,processed,idx,num : integer;
    FileToLoad: String;

begin
  Header:=TStringList.Create;
  processed := 0;
  if (OpenDialog1.Execute) then
   begin
     Orig := TStringList.Create;
     Orig.LoadFromFile(OpenDialog1.FileName);
     Orig.SaveToFile(OpenDialog1.FileName+'.bak');
     while (true) do
      begin
        idx := FindInclude(Orig);
        if (idx=-1) then
         break;
        FileToLoad := ExtractFileDir(OpenDialog1.FileName) + '\' +ExtractString(Orig.Strings[idx]);
        if (FileExists(FileToLoad)) then
          begin
            Header.LoadFromFile(FileToLoad);
            Orig.Delete(idx);
            inc(Processed);
            for  num:=0 to Header.Count-1 do
             begin
               Orig.Insert(Idx+num,Header.Strings[num]);

             end
          end
      end;
   end;
 Memo1.Text := Orig.Text;
  Orig.SaveToFile(OpenDialog1.FileName);
end;

end.

