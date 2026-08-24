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
	int w, h;
	int dialogW, dialogH;

	m_imageBitmap->GetSize(&w, &h);
	GetSize(&dialogW, &dialogH);

	m_dialogToBitmapWidthDiff	= dialogW - w;	// 36
	m_dialogToBitmapHeightDiff	= dialogH - h;	// 95
}

ImageDialog::~ImageDialog()
{
}

bool ImageDialog::ShowModalDialogue()
{
   CentreOnParent();

	int w, h;
	int dialogW, dialogH;

	GetSize(&dialogW, &dialogH);

	w = dialogW - m_dialogToBitmapWidthDiff;
	h = dialogH - m_dialogToBitmapHeightDiff;

	wxBitmap bitmap(m_imageFilename, wxBITMAP_TYPE_JPEG);
	m_image = bitmap.ConvertToImage();
	//m_imageBitmap->GetSize(&w, &h);

	// maintain aspect ratio
	{
		double imgRatio = (double)m_image.GetWidth() / m_image.GetHeight();
		double targetRatio = (double)w / h;

		if (targetRatio > imgRatio)
		{
			w = std::lround(h * imgRatio);
		}
		else
		{
			h = std::lround(w / imgRatio);
		}
	}

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
	if ((m_imageBitmap == nullptr) || (m_image.IsOk() == false))
	{
		return;
	}

	int w, h;
	int dialogW, dialogH;

	GetSize(&dialogW, &dialogH);

	w = dialogW - m_dialogToBitmapWidthDiff;
	h = dialogH - m_dialogToBitmapHeightDiff;

	if (h > 0)
	{
		// maintain aspect ratio
		{
			double imgRatio = (double)m_image.GetWidth() / m_image.GetHeight();
			double targetRatio = (double)w / h;

			if (targetRatio > imgRatio)
			{
				w = std::lround(h * imgRatio);
			}
			else
			{
				h = std::lround(w / imgRatio);
			}
		}

		wxImage shrunkImg = m_image.Scale(w, h, wxIMAGE_QUALITY_HIGH);
		m_imageBitmap->SetBitmap(shrunkImg);
		//m_imageBitmap->Refresh();
		m_topPanel->Refresh();
	}

	event.Skip(); // Call default size behavior for sizers
}
    
void ImageDialog::OnImageBitmapSize(wxSizeEvent& event)
{
	event.Skip(); // Call default size behavior for sizers
}
