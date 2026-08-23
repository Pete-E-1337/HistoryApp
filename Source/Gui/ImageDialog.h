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

private:
   bool IsAffirmativeResult(int32_t dialogueResult);

private:
	std::string m_imageFilename;
	wxImage	m_image;
};

#endif // IMAGE_DIALOG_H