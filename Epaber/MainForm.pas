unit MainForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ExtCtrls, Vcl.StdCtrls, Registry,
  ComPort, ComSignal, Vcl.ComCtrls;

type
  TForm1 = class(TForm)
    imScreen: TImage;
    cmbPorts: TComboBox;
    btnConnect: TButton;
    btnDraw: TButton;
    btnBGClean: TButton;
    btnSendBlack: TButton;
    btnSendRed: TButton;
    comport: TComPort;
    progressSend: TProgressBar;
    dialogFilePicker: TOpenDialog;
    btnLoadRed: TButton;
    btnLoadBlack: TButton;
    procedure FormShow(Sender: TObject);
    procedure imScreenClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnConnectClick(Sender: TObject);
    procedure btnDrawClick(Sender: TObject);
    procedure btnSendRedClick(Sender: TObject);
    procedure btnSendBlackClick(Sender: TObject);
    procedure btnBGCleanClick(Sender: TObject);
    procedure btnLoadRedClick(Sender: TObject);
    procedure btnLoadBlackClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  x_subframes: Integer;
  y_subframes: Integer;
  transmittable: Boolean;

implementation

{$R *.dfm}

procedure TForm1.btnConnectClick(Sender: TObject);
begin
  comport.BaudRate := br38400;
  comport.DeviceName := '\\.\'+cmbPorts.Items[cmbPorts.ItemIndex];
  comport.Active := not comport.Active;
  if comport.Active then
  begin
    btnConnect.Caption := 'HÜLGA';
  end
  else
  begin
    btnConnect.Caption := 'ÜHENDA';
  end;
  transmittable := true;
end;

procedure TForm1.btnLoadRedClick(Sender: TObject);
var
  bmp: TBitmap;
  x: Integer;
  y: Integer;
begin
  if dialogFilePicker.Execute
  then
  begin
     bmp := TBitmap.Create;

     bmp.LoadFromFile(dialogFilePicker.FileName);
     for x := 0 to bmp.Width do
      for y := 0 to bmp.Height do
      begin
        if not (bmp.Canvas.Pixels[x,y] = clWhite) then
        begin
          bmp.Canvas.Pixels[x,y] := clRed;
        end;

      end;
     imScreen.Canvas.Draw(0,0,bmp);
  end
  else ShowMessage('Open file was cancelled');
  //OpenDialog1.Free;
end;

procedure TForm1.btnBGCleanClick(Sender: TObject);
begin
comport.Write('W'+#13,TEncoding.ANSI);
  comport.Flush;
end;

procedure TForm1.btnLoadBlackClick(Sender: TObject);
var
  bmp: TBitmap;
  x: Integer;
  y: Integer;
begin
  if dialogFilePicker.Execute
  then
  begin
     bmp := TBitmap.Create;

     bmp.LoadFromFile(dialogFilePicker.FileName);
     for x := 0 to bmp.Width do
      for y := 0 to bmp.Height do
      begin
        if not (bmp.Canvas.Pixels[x,y] = clWhite) then
        begin
          bmp.Canvas.Pixels[x,y] := clBlack;
        end;

      end;
     imScreen.Canvas.Draw(0,100,bmp);
  end
  else ShowMessage('Open file was cancelled');
  //OpenDialog1.Free;
end;

procedure TForm1.btnDrawClick(Sender: TObject);
begin
 
  comport.Write('R'+#13,TEncoding.ANSI);
  comport.Flush;
end;

procedure TForm1.btnSendBlackClick(Sender: TObject);
var
  x: Integer;
  y: Integer;
  xpos: Integer;
  ypos: Integer;
  subframex: Integer;
  subframey: Integer;
  hascontent: Boolean;
begin
  progressSend.Position := 0;
  for xpos := 0 to x_subframes do begin
    for ypos := 0 to y_subframes do begin
      subframex := xpos*40;
      subframey := ypos*40;
      hascontent := false;
      for x := 0 to 40 do begin
        for y := 0 to 40 do begin
          progressSend.StepBy(1);
          Application.ProcessMessages;

          if imscreen.Canvas.Pixels[subframex+x,subframey+y] = clBlack then
          begin
              hascontent := true;
              comport.Write('S',TEncoding.ANSI);
              comport.WriteByte(x);
              comport.WriteByte(y);
              comport.Write(''+#13,TEncoding.ANSI);
              comport.Flush();
              comport.Flush();
              Sleep(100);
          end;

        end;
      end;
      if hascontent then
      begin
        comport.Write('DB',TEncoding.ANSI);
        comport.WriteByte(subframex);
        comport.WriteByte(subframey);
        comport.Write(''+#13,TEncoding.ANSI);
        comport.Flush();
        Sleep(50);
      end;

  end;
end;
end;
procedure TForm1.btnSendRedClick(Sender: TObject);
var
  x: Integer;
  y: Integer;
  xpos: Integer;
  ypos: Integer;
  subframex: Integer;
  subframey: Integer;
  hascontent: Boolean;
begin
  progressSend.Position := 0;
  for xpos := 0 to x_subframes do begin
    for ypos := 0 to y_subframes do begin
      subframex := xpos*40;
      subframey := ypos*40;
      hascontent := false;
      for x := 0 to 40 do begin
        for y := 0 to 40 do begin
          progressSend.StepBy(1);
          Application.ProcessMessages;

          if imScreen.Canvas.Pixels[subframex+x,subframey+y] = clRed then
          begin
              hascontent := true;
              comport.Write('S',TEncoding.ANSI);
              comport.WriteByte(x);
              comport.WriteByte(y);
              comport.Write(''+#13,TEncoding.ANSI);
              comport.Flush();
              comport.Flush();
              Sleep(100);
          end;

        end;
      end;
      if hascontent then
      begin
        comport.Write('DR',TEncoding.ANSI);
        comport.WriteByte(subframex);
        comport.WriteByte(subframey);
        comport.Write(''+#13,TEncoding.ANSI);
        comport.Flush();
        Sleep(50);
      end;

  end;
end;

end;

procedure TForm1.FormCreate(Sender: TObject);
 var
 reg:  TRegistry;
 I:Integer;
begin
   Reg := TRegistry.Create;
  try
    Reg.RootKey := HKEY_LOCAL_MACHINE;
    if Reg.OpenKeyReadOnly('\HARDWARE\DEVICEMAP\SERIALCOMM') then
    begin
      Reg.GetValueNames(cmbPorts.Items);
      for I := 0 to cmbPorts.Items.Count - 1 do
          cmbPorts.Items[i] := Reg.ReadString(cmbPorts.Items[i]);
    end;
    cmbPorts.Sorted := true;
  finally
    Reg.Free;
  end;
  imScreen.Height := 250;
  imScreen.Width := 120;
  x_subframes := imScreen.Width div 40;
  y_subframes := imScreen.Height div 40;
  progressSend.Max := imScreen.Height*imScreen.Width;
end;
procedure TForm1.FormShow(Sender: TObject);
begin
  with imScreen do
  begin
    Canvas.Pen.Color := clRed;
    Canvas.Brush.Color := clRed;
    Canvas.Brush.Style := bsSolid;
    //Canvas.Pixels[100,100] := clRed;
  end;
end;

procedure TForm1.imScreenClick(Sender: TObject);
var
  ptmouse : TPoint;
begin
  ptmouse := Mouse.CursorPos;
  ptmouse := ScreenToClient(ptmouse);

   with imScreen do
  begin
    Canvas.Pen.Color := clRed;
    Canvas.Brush.Color := clRed;
    Canvas.Brush.Style := bsSolid;
  end;

end;

end.
