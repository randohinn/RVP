object Form1: TForm1
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'E-Paber'
  ClientHeight = 313
  ClientWidth = 334
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object imScreen: TImage
    Left = 0
    Top = 8
    Width = 120
    Height = 250
    OnClick = imScreenClick
  end
  object cmbPorts: TComboBox
    Left = 8
    Top = 264
    Width = 65
    Height = 21
    TabOrder = 0
  end
  object btnConnect: TButton
    Left = 79
    Top = 262
    Width = 75
    Height = 25
    Caption = #220'henda'
    TabOrder = 1
    OnClick = btnConnectClick
  end
  object btnDraw: TButton
    Left = 232
    Top = 235
    Width = 99
    Height = 52
    Caption = 'JOONISTA'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = btnDrawClick
  end
  object btnBGClean: TButton
    Left = 232
    Top = 142
    Width = 99
    Height = 25
    Caption = 'Taust puhtaks'
    TabOrder = 3
    OnClick = btnBGCleanClick
  end
  object btnSendBlack: TButton
    Left = 232
    Top = 173
    Width = 99
    Height = 25
    Caption = 'Saada must'
    TabOrder = 4
    OnClick = btnSendBlackClick
  end
  object btnSendRed: TButton
    Left = 232
    Top = 204
    Width = 99
    Height = 25
    Caption = 'Saada punane'
    TabOrder = 5
    OnClick = btnSendRedClick
  end
  object progressSend: TProgressBar
    Left = 0
    Top = 293
    Width = 331
    Height = 17
    TabOrder = 6
  end
  object btnLoadRed: TButton
    Left = 232
    Top = 5
    Width = 99
    Height = 23
    Caption = 'Lae pilt punaselt'
    TabOrder = 7
    OnClick = btnLoadRedClick
  end
  object btnLoadBlack: TButton
    Left = 232
    Top = 34
    Width = 99
    Height = 23
    Caption = 'Lae pilt mustalt'
    TabOrder = 8
    OnClick = btnLoadBlackClick
  end
  object comport: TComPort
    Active = False
    BaudRate = br38400
    DeviceName = '\\.\COM15'
    Options = []
    Parity = paNone
    StopBits = sb1
    Left = 144
    Top = 32
  end
  object dialogFilePicker: TOpenDialog
    Left = 168
    Top = 104
  end
end
