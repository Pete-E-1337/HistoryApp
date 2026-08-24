#ifndef IMAGE_DIALOG_H
#define IMAGE_DIALOG_H

#include "./GeneratedFiles/History.h"
//#include <vector>

class ImageDialog : public History::ImageDialog
{
public:
	ImageDialog(wxWindow* parent);
   ~ImageDialog();

   bool ShowModalDialogue();
	void SetImageFilename(std::string imageFilename) { m_imageFilename = imageFilename; }
	std::string GetImageFilename() { return m_imageFilename; }

protected:
	//virtual void OnPortChoice(wxCommandEvent& event) override;
	//virtual void OnBaudRateChoice(wxCommandEvent& event) override;
	virtual void OnImageSdbSizerOKButtonClick(wxCommandEvent& event) override;
	virtual void OnImageDialogSize(wxSizeEvent& event) override;
	virtual void OnImageBitmapSize(wxSizeEvent& event) override;

private:
   bool IsAffirmativeResult(int32_t dialogueResult);

private:
	std::string	m_imageFilename;
	wxImage		m_image;
//	int			m_bottomPanelHeight;		// required to deal with a GUI bug of the panel disappearing during resizing :/

	// Use these to resize the bitmap. Using the bitmap size itself proved buggy.
	int			m_dialogToBitmapWidthDiff;
	int			m_dialogToBitmapHeightDiff;
};

#endif // IMAGE_DIALOG_H