{
cout << endl << "Welcome to the MyStyle rootlogon.C" << endl;
TStyle *myStyle= new TStyle("MyStyle","My style");
myStyle->SetPalette(1,0);
// use plain black on white colors
 Int_t icol=0;
myStyle->SetFrameBorderMode(icol);
myStyle->SetCanvasBorderMode(icol);
myStyle->SetPadBorderMode(icol);
myStyle->SetPadColor(icol);
myStyle->SetCanvasColor(icol);
myStyle->SetStatColor(icol);
//myStyle->SetFillColor(icol);
myStyle->SetFuncColor(kRed);
myStyle->SetFuncWidth(1);
// set the paper & margin sizes
myStyle->SetPaperSize(20,26);
myStyle->SetPadTopMargin(0.05);
myStyle->SetPadRightMargin(0.05);
myStyle->SetPadBottomMargin(0.16);
myStyle->SetPadLeftMargin(0.12);
// use large fonts
//Int_t font=72;
Int_t font=42;
Double_t tsize=0.04;
myStyle->SetTextFont(font);
myStyle->SetTextSize(tsize);
myStyle->SetLabelFont(font,"x");
myStyle->SetTitleFont(font,"x");
myStyle->SetLabelFont(font,"y");
myStyle->SetTitleFont(font,"y");
myStyle->SetLabelFont(font,"z");
myStyle->SetTitleFont(font,"z");
myStyle->SetLabelSize(tsize,"x");
myStyle->SetTitleSize(tsize,"x");
myStyle->SetLabelSize(tsize,"y");
myStyle->SetTitleSize(tsize,"y");
myStyle->SetLabelSize(tsize,"z");
myStyle->SetTitleSize(tsize,"z");

myStyle->SetTitleOffset(1.3,"x");
myStyle->SetTitleOffset(1.5,"y");

//use bold lines and markers
//myStyle->SetMarkerStyle(20);
//myStyle->SetMarkerSize(1.2);
//myStyle->SetHistLineWidth(1.);
myStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
//get rid of X error bars and y error bar caps
//myStyle->SetErrorX(0.001);
//do not display any of the standard histogram decorations
myStyle->SetOptTitle(0);
//myStyle->SetOptStat(111111);
myStyle->SetOptStat(0);
//myStyle->SetOptFit(111111);
myStyle->SetOptFit(0);
// put tick marks on top and RHS of plots
myStyle->SetPadTickX(1);
myStyle->SetPadTickY(1);
gROOT->SetStyle("Plain");
//gStyle->SetPadTickX(1);
//gStyle->SetPadTickY(1);
}