#include "ImageDialog.h"

//#include <wx/wx.h>
//#include <wx/numformatter.h>
//#include <math.h>
//#include <fstream>
//
#include <SVSLibrary/Std.h>
#include <SVSLibrary/Compiler/Breakpoint.h>
//#include <SVSLibrary/StringUtilities.h>

SVS_WARNING_DISABLE(4189) // local variable is initialized but not referenced

ImageDialog::ImageDialog(wxWindow* parent) :
	History::ImageDialog(parent)
{
	m_bottomPanelHeight = m_bottomPanel->GetSize().GetHeight();
}

ImageDialog::~ImageDialog()
{
}

bool ImageDialog::ShowModalDialogue()
{
   CentreOnParent();

	int w, h;

	wxBitmap bitmap(m_imageFilename, wxBITMAP_TYPE_JPEG);
	m_image = bitmap.ConvertToImage();
	m_imageBitmap->GetSize(&w, &h);
	wxImage shrunkImg = m_image.Scale(w, h, wxIMAGE_QUALITY_HIGH);
	m_imageBitmap->SetBitmap(shrunkImg);
	m_imageBitmap->Refresh();

   return (IsAffirmativeResult(ShowModal()));
}

bool ImageDialog::IsAffirmativeResult(int32_t dialogueResult)
{
   switch (dialogueResult)
   {
      case wxID_OK:
      case wxID_YES:
      case wxID_APPLY:
      case wxID_YESTOALL:
         return (true);

      default:
         return (false);
   }
}

void ImageDialog::OnImageSdbSizerOKButtonClick(wxCommandEvent& event)
{
   NOT_USED(event);

	EndModal(wxID_OK);
}

void ImageDialog::OnImageDialogSize(wxSizeEvent& event)
{
	if (!m_imageBitmap || !m_image.IsOk())
		return;
    
	int w, h;

	m_imageBitmap->GetSize(&w, &h);

//int h1 = m_topPanel->GetSize().GetHeight();
//int h2 = m_imageBitmap->GetSize().GetHeight();
//int h3 = m_imageBitmap->GetClientSize().GetHeight();
//
//int h4 = m_bottomPanel->GetSize().GetHeight();
//int h5 = m_bottomPanel->GetSize().GetY();

//int bottomPanelWidth, bottomPanelHeight;
//wxSize size = event.GetSize();
//m_bottomPanel->GetSize(&bottomPanelWidth, &bottomPanelHeight);
//w = size.GetWidth();
//h = size.GetHeight() - 200;

	h = h - m_bottomPanelHeight; // For some unknown reason it doesn't keep the "ok" button visible. Add this code to compensate :/

	if (h > 0)
	{
		wxImage shrunkImg = m_image.Scale(w, h, wxIMAGE_QUALITY_HIGH);
		m_imageBitmap->SetBitmap(shrunkImg);
		m_imageBitmap->Refresh();
	}

	event.Skip(); // Call default size behavior for sizers
}
    
