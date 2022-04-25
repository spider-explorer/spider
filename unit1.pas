unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, StdCtrls, ComCtrls,
  Menus, ExtCtrls, Process, fphttpclient, opensslsockets, TypInfo, fpjson,
  jsonparser,
  ATShellBase,
  ATShellTreeview,
  formexplorer,
  formmenubox;

type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    GroupBox1: TGroupBox;
    GroupBox2: TGroupBox;
    Label1: TLabel;
    ListView1: TListView;
    MenuItemOne: TMenuItem;
    Panel1: TPanel;
    Panel2: TPanel;
    PopupMenu1: TPopupMenu;
    Splitter1: TSplitter;
    StatusBar1: TStatusBar;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure MenuItemOneClick(Sender: TObject);
    procedure PopupMenu1Popup(Sender: TObject);
    procedure on__PopupMenu1_dynamic(Sender: TObject);
    procedure ExplorerClick(const fn: string; Kind: TATShellTreeviewClick);
  private

  public

  end;

var
  Form1: TForm1;
  exp: TfmExplorer;
  //exp: TATShellTreeview;
  m_repoRoot: String;

implementation

function ReadURLGet(url:string):string;
begin
with TFPHTTPClient.Create(nil) do
try
  ReadURLGet := Get(url);
finally
  Free;
end;
end;

function ReadEnv():string;
var
  sExe: String;
  hprocess: TProcess;
  OutputLines: TStringList;
begin
  sExe := ExtractFilePath(Application.ExeName) + 'cmd\spider-console.exe';
  OutputLines:=TStringList.Create;
  hProcess := TProcess.Create(nil);
  hProcess.Executable := sExe;
  hprocess.Parameters.Add('--env');
  hProcess.Options := hProcess.Options + [poNoConsole, poWaitOnExit, poUsePipes];
  hProcess.Execute;
  OutputLines.LoadFromStream(hprocess.Output);
  result := OutputLines.Text;
  hProcess.Free;
  OutputLines.Free;
end;

{$R *.lfm}

{ TForm1 }

procedure TForm1.Button1Click(Sender: TObject);
var
  Itm: TListItem;
begin
  ShowMessage('helloハロー©');
  Itm := ListView1.Items.Add;
  Itm.Caption := 'abc';
  Itm.SubItems.Add('xyz');
  Itm := ListView1.Items.Add;
  Itm.Caption := 'あいう';
  Itm.SubItems.Add('かきく');
  //ListView1.AddItem(Itm);
end;

procedure TForm1.FormCreate(Sender: TObject);
var
  json: String;
  jData : TJSONData;
  jItem : TJSONData;
begin
  ATShellOptions.DirOfIcons:= ExtractFilePath(Application.ExeName)+'vscode_16x16';

  exp:= TfmExplorer.Create(Self);
  //exp:= TATShellTreeview.Create(Self);
  exp.Parent:= Panel1;
  exp.Align:= alClient;
  exp.BorderStyle:= bsNone;
  exp.Folder:='D:\\Users\\user\\Documents\\.repo\\spider';
  //exp.PopupMode:=pmExplicit;
  //exp.PopupMenu := PopupMenu1;
  exp.Show;

  //ATShellIcons.OnDetect:= @ExplorerGetLexer;
  exp.OnItemClick:= @ExplorerClick;
  //exp.OnGetTabs:= @ExplorerGetTabs;
  //exp.OnGetTabProp:= @ExplorerGetTabProp;
  //exp.OnTabSelect:= @ExplorerTabSelect;
  //exp.OnTabClose:= @ExplorerTabClose;

  //ATShellOptions.DirOfIcons:='D:\\Users\\user\\Documents\\.repo\\spider\\vscode_16x16';
  //Tree:= TATShellTreeview.Create(Self);
  //Tree.OnClick:=@on__ExplorerClick;
  //Tree.Parent:= Panel1;
  //Tree.Align:= alClient;
  //Tree.Folder:= 'D:\\Users\\user\\Documents\\.repo\\spider';

  //Form1.AutoSize := True;
  json := ReadEnv();
  //ShowMessage(json);
  jData := GetJSON(json);
  //ShowMessage(IntToStr(jData.Count));
  jItem := jData.FindPath('repoRoot');
  m_repoRoot := jItem.AsString;
  Label1.Caption := m_repoRoot;
  jData.Free;
end;

procedure TForm1.MenuItemOneClick(Sender: TObject);
var
  item: TListItem;
begin
  ShowMessage('One');
  ShowMessage(Sender.ClassType.ClassName);
  item := ListView1.Selected;
  if item <> nil then begin
    ShowMessage(item.Caption + item.SubItems.Text);
  end;
end;

procedure TForm1.PopupMenu1Popup(Sender: TObject);
var
   item: TListItem;
   i: Integer;
   NewMenu: TMenuItem;
begin
   item := ListView1.selected;
   if item = nil then abort;
   for i := 0 to PopupMenu1.Items.Count-1 do begin
       PopupMenu1.Items[i].Free;
   end;
   PopupMenu1.Items.Clear;
   NewMenu:=TMenuItem.Create(PopupMenu1);
   NewMenu.Caption := '動的メニュー';
   NewMenu.OnClick := @on__PopupMenu1_dynamic;
   PopupMenu1.Items.Add(NewMenu);
end;

procedure TForm1.on__PopupMenu1_dynamic(Sender: TObject);
var
  item: TListItem;
begin
  ShowMessage('Dynamic');
  ShowMessage(Sender.ClassType.ClassName);
  item := ListView1.Selected;
  if item <> nil then begin
    ShowMessage(item.Caption + item.SubItems.Text);
  end;
end;

procedure TForm1.ExplorerClick(const fn: string; Kind: TATShellTreeviewClick);
const
  sKind: array[TATShellTreeviewClick] of string = ('-', 'click', 'dbl-click', 'fold', 'unfold');
begin
  StatusBar1.SimpleText:= '"'+fn+'", '+sKind[Kind];

  case Kind of
    astcFileClick:
      //ShowMessage('Single-Click:'+fn);
      ;
    astcFileDblClick:
      ShowMessage('Double-Click:'+fn);
  end;
end;

end.

