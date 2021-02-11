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
  ComPort1.BaudRate := br38400;
  ComPort1.DeviceName := '\\.\'+ComboBox1.Items[ComboBox1.ItemIndex];
  ComPort1.Active := not ComPort1.Active;
  if ComPort1.Active then
  begin
    Button1.Caption := 'HÜLGA';
  end
  else
  begin
    Button1.Caption := 'ÜHENDA';
  end;
  transmittable := true;
end;

procedure TForm1.btnLoadRedClick(Sender: TObject);
var
  bmp: TBitmap;
  x: Integer;
  y: Integer;
begin
  if OpenDialog1.Execute
  then
  begin
     bmp := TBitmap.Create;

     bmp.LoadFromFile(OpenDialog1.FileName);
     for x := 0 to bmp.Width do
      for y := 0 to bmp.Height do
      begin
        if not (bmp.Canvas.Pixels[x,y] = clWhite) then
        begin
          bmp.Canvas.Pixels[x,y] := clRed;
        end;

      end;
     Image1.Canvas.Draw(0,0,bmp);
  end
  else ShowMessage('Open file was cancelled');
  //OpenDialog1.Free;
end;

procedure TForm1.btnBGCleanClick(Sender: TObject);
begin
ComPort1.Write('W'+#13,TEncoding.ANSI);
  Comport1.Flush;
end;

procedure TForm1.btnLoadBlackClick(Sender: TObject);
var
  bmp: TBitmap;
  x: Integer;
  y: Integer;
begin
  if OpenDialog1.Execute
  then
  begin
     bmp := TBitmap.Create;

     bmp.LoadFromFile(OpenDialog1.FileName);
     for x := 0 to bmp.Width do
      for y := 0 to bmp.Height do
      begin
        if not (bmp.Canvas.Pixels[x,y] = clWhite) then
        begin
          bmp.Canvas.Pixels[x,y] := clBlack;
        end;

      end;
     Image1.Canvas.Draw(0,100,bmp);
  end
  else ShowMessage('Open file was cancelled');
  //OpenDialog1.Free;
end;

procedure TForm1.btnDrawClick(Sender: TObject);
begin
 
  ComPort1.Write('R'+#13,TEncoding.ANSI);
  Comport1.Flush;
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
  ProgressBar1.Position := 0;
  for xpos := 0 to x_subframes do begin
    for ypos := 0 to y_subframes do begin
      subframex := xpos*40;
      subframey := ypos*40;
      hascontent := false;
      for x := 0 to 40 do begin
        for y := 0 to 40 do begin
          ProgressBar1.StepBy(1);
          Application.ProcessMessages;

          if Image1.Canvas.Pixels[subframex+x,subframey+y] = clBlack then
          begin
              hascontent := true;
              ComPort1.Write('S',TEncoding.ANSI);
              ComPort1.WriteByte(x);
              ComPort1.WriteByte(y);
              ComPort1.Write(''+#13,TEncoding.ANSI);
              Comport1.Flush();
              Comport1.Flush();
              Sleep(100);
          end;

        end;
      end;
      if hascontent then
      begin
        ComPort1.Write('DB',TEncoding.ANSI);
        ComPort1.WriteByte(subframex);
        ComPort1.WriteByte(subframey);
        ComPort1.Write(''+#13,TEncoding.ANSI);
        Comport1.Flush();
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
  ProgressBar1.Position := 0;
  for xpos := 0 to x_subframes do begin
    for ypos := 0 to y_subframes do begin
      subframex := xpos*40;
      subframey := ypos*40;
      hascontent := false;
      for x := 0 to 40 do begin
        for y := 0 to 40 do begin
          ProgressBar1.StepBy(1);
          Application.ProcessMessages;

          if Image1.Canvas.Pixels[subframex+x,subframey+y] = clRed then
          begin
              hascontent := true;
              ComPort1.Write('S',TEncoding.ANSI);
              ComPort1.WriteByte(x);
              ComPort1.WriteByte(y);
              ComPort1.Write(''+#13,TEncoding.ANSI);
              Comport1.Flush();
              Comport1.Flush();
              Sleep(100);
          end;

        end;
      end;
      if hascontent then
      begin
        ComPort1.Write('DR',TEncoding.ANSI);
        ComPort1.WriteByte(subframex);
        ComPort1.WriteByte(subframey);
        ComPort1.Write(''+#13,TEncoding.ANSI);
        Comport1.Flush();
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
      Reg.GetValueNames(ComboBox1.Items);
      for I := 0 to ComboBox1.Items.Count - 1 do
          ComboBox1.Items[i] := Reg.ReadString(ComboBox1.Items[i]);
    end;
    ComboBox1.Sorted := true;
  finally
    Reg.Free;
  end;
  Image1.Height := 250;
  Image1.Width := 120;
  x_subframes := Image1.Width div 40;
  y_subframes := Image1.Height div 40;
  ProgressBar1.Max := Image1.Height*Image1.Width;
end;
procedure TForm1.FormShow(Sender: TObject);
begin
  with Image1 do
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

   with Image1 do
  begin
    Canvas.Pen.Color := clRed;
    Canvas.Brush.Color := clRed;
    Canvas.Brush.Style := bsSolid;
  end;

end;

end.
