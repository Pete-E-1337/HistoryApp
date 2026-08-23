#include "ImageDialog.h"

//#include <wx/wx.h>
//#include <wx/numformatter.h>
//#include <math.h>
//#include <fstream>
//
#include <SVSLibrary/Std.h>
#include <SVSLibrary/Compiler/Breakpoint.h>
//#include <SVSLibrary/StringUtilities.h>

ImageDialog::ImageDialog(wxWindow* parent) :
	History::ImageDialog(parent)
{
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
//	m_imageBitmap->Refresh();

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

	//int bottomPanelWidth, bottomPanelHeight;
	//wxSize size = event.GetSize();
	//m_bottomPanel->GetSize(&bottomPanelWidth, &bottomPanelHeight);
	//w = size.GetWidth();
	//h = size.GetHeight() - 200;
	h = h - 26;

	if (h > 0)
	{
		wxImage shrunkImg = m_image.Scale(w, h, wxIMAGE_QUALITY_HIGH);
		m_imageBitmap->SetBitmap(shrunkImg);
	}

	event.Skip(); // Call default size behavior for sizers
}
    
