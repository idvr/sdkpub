unit Ai03;
{
ULAI03.DPR================================================================

 File:                         AI03.PAS

 Library Call Demonstrated:    cbAInScan(), BACKGROUND mode

 Purpose:                      Scans a range of A/D Input Channels and stores
                               the sample data in an array.

 Demonstration:                Displays the analog input on eight channels.

 Other Library Calls:          cbGetStatus%()
                               cbStopBackground%()
                               cbErrHandling%()

 Special Requirements:         Board 0 must have an A/D converter.
                               Analog signals on eight input channels.

 (c) Copyright 1995 - 2002, Measurement Computing Corp.
 All rights reserved.
==========================================================================
}
interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, StdCtrls, ExtCtrls, cbw;

type
  TfrmAInScan = class(TForm)
    tmrCheckStatus: TTimer;
    cmdStart: TButton;
    cmdQuit: TButton;
    Memo1: TMemo;
    lblShowCount: TLabel;
    lblShowIndex: TLabel;
    lblShowStat: TLabel;
    cmdStop: TButton;
    procedure cmdStartClick(Sender: TObject);
    procedure cmdQuitClick(Sender: TObject);
    procedure cmdStopClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure tmrCheckStatusTimer(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
   frmAInScan: TfrmAInScan;

implementation

{$R *.DFM}

var
   ULStat:                   Integer;
   MemHandle:                Integer;
   ErrReporting:             Integer;
   ErrHandling:              Integer;
   ADData:                   array[0..10000] of Word;
   PrnNum:                   Integer;
   Rate:                     LongInt;
   NumPoints:                LongInt;
   CurCount, CurIndex:       LongInt;
   FirstPoint:               LongInt;
   UserTerm:                 Boolean;
   RevLevel:                 Single;
    Status:                  SmallInt;

const
   BoardNum:                 Integer = 0;
   Count:                    LongInt = 10000;
   TargetRate:               LongInt = 1000;
   LowChan:                  Integer = 0;
   HighChan:                 Integer = 1;
   Range:                    LongInt = BIP5VOLTS;
   Options:                  Integer = CONVERTDATA + BACKGROUND + SINGLEIO;

procedure TfrmAInScan.FormCreate(Sender: TObject);
begin
  {declare Revision Level}
   RevLevel := CURRENTREVNUM;
   ULStat := cbDeclareRevision(RevLevel);

   {
   set up internal error handling for the Universal Library
   }
   ErrReporting := PRINTALL;       {set Universal Library to print all errors}
   ErrHandling := STOPALL;         {set Universal Library to stop on errors}
   ULStat := cbErrHandling(ErrReporting, ErrHandling);
   {
   set up a buffer in Windows to contain the data
   }
   MemHandle := cbWinBufAlloc (Count);
   Memo1.Text := 'Click start to acquire data';
end;

procedure TfrmAInScan.cmdStartClick(Sender: TObject);
begin
   {
    Collect the values with cbAInScan()
        Parameters:  (see Initialization section)
            BoardNum    :the number used by CB.CFG to describe this board
            LowChan     :low channel of the scan
            HighChan    :high channel of the scan
            Count       :the total number of A/D samples to collect
            Rate        :sample rate in samples per second
            Gain        :the gain for the board
            MemHandle   :Windows memory set up with cbWinBufAlloc()
            Options     :data collection options
   }
   Memo1.Text := ' ';
   UserTerm := False;
   Rate := TargetRate;
   PrnNum := 1000;
   ULStat := cbAInScan(BoardNum, LowChan, HighChan,
                    Count, Rate, Range, MemHandle, Options);
   If ULStat <> 0 then exit;
   tmrCheckStatus.Enabled := True;
end;

procedure TfrmAInScan.tmrCheckStatusTimer(Sender: TObject);
var 
   index:      Integer;
begin
   {
    This timer will check the status of the background data collection

    Parameters:
      BoardNum    :the number used by CB.CFG to describe this board
      Status      :current status of the background data collection
      CurCount    :current number of samples collected
      CurIndex    :index to the data buffer pointing to the start of the
                   most recently collected scan
      FunctionType: A/D operation (AIFUNCTION)

   }
   ULStat := cbGetStatus(BoardNum, Status, CurCount, CurIndex,AIFUNCTION);
   If ULStat <> 0 Then Exit;

   lblShowCount.Caption := 'Count:  ' + IntToStr(CurCount);
   lblShowIndex.Caption := 'Index:  ' + IntToStr(CurIndex);
   {
    Check if the background operation has finished. If it has, then
    transfer the data from the memory buffer set up by Windows to an
    array for use by Delphi
    The BACKGROUND operation must be explicitly stopped
   }
   If (Status = RUNNING) And (UserTerm = False) Then
   begin
      lblShowStat.Caption := 'Status:  Running';
      If (CurCount > PrnNum) then
      begin
         Memo1.Text := '';
         NumPoints := 2;
         FirstPoint := PrnNum;
         ULStat := cbWinBufToArray (MemHandle, ADData[0], FirstPoint, NumPoints);
         If ULStat <> 0 then exit;
         Memo1.Lines.Add (Format('Sample number %d:   %d',[PrnNum, ADData[0]]));
         Memo1.Lines.Add (Format('Sample number %d:   %d',[PrnNum + 1, ADData[1]]));
         Inc(PrnNum, 1000);
      end
   end
   Else
   begin
      ULStat := cbStopBackground(BoardNum, AIFUNCTION);
      If ULStat <> 0 Then Exit;
      lblShowStat.Caption := 'Status:  Idle';
      ULStat := cbGetStatus(BoardNum, Status, CurCount, CurIndex,AIFUNCTION);
      If ULStat <> 0 Then Exit;
      lblShowCount.Caption := 'Count:  ' + IntToStr(CurCount);
      lblShowIndex.Caption := 'Index:  ' + IntToStr(CurIndex);
      tmrCheckStatus.Enabled := False;
      If MemHandle = 0 Then Exit;

      {
      Transfer the data from the Windows buffer to an array.  This data
      could also be accessed directly using a pointer.
      }
      FirstPoint := 0;
      ULStat := cbWinBufToArray (MemHandle, ADData[0], FirstPoint, Count);
      If ULStat <> 0 then exit;
      Memo1.Text := '';
      index := 0;
      while index < 8 do
      begin
         Memo1.Lines.Add (Format('Channel 0, sample number %d:  %d', [index, ADData[index]]));
         Memo1.Lines.Add (Format('Channel 1, sample number %d:  %d', [index + 1, ADData[index + 1]]));
         Memo1.Lines.Add ('');
         Inc(index, 2);
      end;
   end;
end;

procedure TfrmAInScan.cmdStopClick(Sender: TObject);
begin
   UserTerm := True;      {ends background operation}
end;

procedure TfrmAInScan.cmdQuitClick(Sender: TObject);
begin
   tmrCheckStatus.Enabled := False;
   ULStat := cbStopBackground(BoardNum, AIFUNCTION);
   If ULStat <> 0 Then Exit;
   ULStat := cbWinBufFree (MemHandle);
   Close;
end;

end.
