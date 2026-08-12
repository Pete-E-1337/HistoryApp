#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>

class Settings
{
public:
   Settings();
   virtual ~Settings();

   bool Load(const std::string& filename);
   bool Save(const std::string& filename);

private:
	//typedef struct LicenseDetails
	//{
	//	bool trialMode									= false;
	//	bool rentalMode								= false;
	//	bool erased										= false;
	//	int numCameras									= 0;
	//	int unusualBehaviours						= 0;
	//	int tripWires									= 0;
	//	int leftObjects								= 0;
	//	int trexTier1s									= 0;
	//	int trexTier2s									= 0;
	//	int deFences									= 0;
	//	int loiterings									= 0;
	//	int deepLearnings								= 0;
	//} LicenseDetails;

	//typedef struct LicenseData
	//{
	//	// Identification
	//	std::string userID							= "";
	//	std::string siteID							= "";
	//	std::string siteDescription				= "";
	//	std::string reSellerID						= "";
	//	std::string requestID						= "";
	//	std::string productID						= "";
	//	std::string versionID						= "";
	//	std::vector<std::string> cpuID			= {};
	//	std::vector<std::string> gpuID			= {};
	//	std::vector<std::string> networkdID		= {};
	//	std::vector<std::string> hardDriveID	= {};
	//	std::string systemSerial					= "";
	//	std::string systemName						= "";
	//	std::string osName							= "";
	//	std::string osVersion						= "";
	//	std::string systemID							= "";

	//	// Record
	//	std::string currentDate						= "";
	//	std::string firstRequestDate				= "";
	//	std::string lastRequestDate				= "";
	//	std::string installationDate				= "";
	//	std::string expiryDate						= "";
	//	std::string requestDate						= "";
 //
	//	// Requested
	//	LicenseDetails requested;

	//	// Licensed
	//	LicenseDetails licensed;
	//} LicenseData;

   typedef std::string JsonStr;  // Even though it's just a string, type it to keep us reminded that it is a string holding json formatted text

	bool ConvertJsonStrToData(const JsonStr& jsonStr);
	bool ConvertDataToJsonStr(JsonStr& jsonStr);

	std::string LoadFileToString(const std::string& filename);
	bool SaveStringToFile(const std::string& str, const std::string& filename);

public:
};

#endif // SETTINGS_H

