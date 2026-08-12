#include "Settings.h"

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

#pragma warning (disable:4189) // local variable is initialized but not referenced

Settings::Settings()
{
}

Settings::~Settings()
{
}

bool Settings::Load(const std::string& filename)
{
	std::string jsonStr = LoadFileToString(filename);

	if (jsonStr.empty() == true)
	{
		printf("Settings::Load error: Could not load file \"%s\".\n", filename.c_str()); 
		return "";
	}

   ConvertJsonStrToData(jsonStr);

   return true;
}

bool Settings::Save(const std::string& filename)
{
   std::string jsonStr;

   ConvertDataToJsonStr(jsonStr);

	if (SaveStringToFile(jsonStr, filename) == false)
	{
		printf("Settings::Save error: Could not save file \"%s\".\n", filename.c_str()); 
	}

   return true;
}

bool Settings::ConvertJsonStrToData(const JsonStr& jsonStr)
{
	rapidjson::Document document;

	document.Parse(jsonStr.c_str(), jsonStr.size());    // rapidjson example: buffer and size to document

	if (document.HasParseError() || document.ObjectEmpty())
	{
		return false;
	}

	if (document.HasMember("settings") == true)
	{
		const rapidjson::Value& settings = document["settings"];

		//auto const userID = settings.FindMember("userID");

		//if (userID != settings.MemberEnd() && userID->value.IsString())
		//{
		//	ld.userID = userID->value.GetString();
		//}

		//auto const siteID = settings.FindMember("siteID");

		//if (siteID != settings.MemberEnd() && siteID->value.IsString())
		//{
		//	ld.siteID = siteID->value.GetString();
		//}

		//auto const siteDescription = settings.FindMember("siteDescription");

		//if (siteDescription != settings.MemberEnd() && siteDescription->value.IsString())
		//{
		//	ld.siteDescription = siteDescription->value.GetString();
		//}

		//auto const reSellerID = settings.FindMember("reSellerID");

		//if (reSellerID != settings.MemberEnd() && reSellerID->value.IsString())
		//{
		//	ld.reSellerID = reSellerID->value.GetString();
		//}

		//auto const requestID = settings.FindMember("requestID");

		//if (requestID != settings.MemberEnd() && requestID->value.IsString())
		//{
		//	ld.requestID = requestID->value.GetString();
		//}

		//auto const productID = settings.FindMember("productID");

		//if (productID != settings.MemberEnd() && productID->value.IsString())
		//{
		//	ld.productID = productID->value.GetString();
		//}

		//auto const versionID = settings.FindMember("versionID");

		//if (versionID != settings.MemberEnd() && versionID->value.IsString())
		//{
		//	ld.versionID = versionID->value.GetString();
		//}

		//auto const cpuID = settings.FindMember("cpuID");

		//if (cpuID != settings.MemberEnd() && cpuID->value.IsArray())
		//{
		//	ld.cpuID.clear();
		//	const auto cpuArray = cpuID->value.GetArray();

		//	for (const auto& cpuArrayElement : cpuArray)
		//	{
		//		if (cpuArrayElement.IsString() == true)
		//		{
		//			ld.cpuID.push_back(cpuArrayElement.GetString());
		//		}
		//	}
		//}

		//auto const gpuID = settings.FindMember("gpuID");

		//if (gpuID != settings.MemberEnd() && gpuID->value.IsArray())
		//{
		//	ld.gpuID.clear();
		//	const auto gpuArray = gpuID->value.GetArray();

		//	for (const auto& gpuArrayElement : gpuArray)
		//	{
		//		if (gpuArrayElement.IsString() == true)
		//		{
		//			ld.gpuID.push_back(gpuArrayElement.GetString());
		//		}
		//	}
		//}

		//auto const networkdID = settings.FindMember("networkdID");

		//if (networkdID != settings.MemberEnd() && networkdID->value.IsArray())
		//{
		//	ld.networkdID.clear();
		//	const auto networkdIDArray = networkdID->value.GetArray();

		//	for (const auto& networkdIDArrayElement : networkdIDArray)
		//	{
		//		if (networkdIDArrayElement.IsString() == true)
		//		{
		//			ld.networkdID.push_back(networkdIDArrayElement.GetString());
		//		}
		//	}
		//}

		//auto const hardDriveID = settings.FindMember("hardDriveID");

		//if (hardDriveID != settings.MemberEnd() && hardDriveID->value.IsArray())
		//{
		//	ld.hardDriveID.clear();
		//	const auto hardDriveIDArray = hardDriveID->value.GetArray();

		//	for (const auto& hardDriveIDArrayElement : hardDriveIDArray)
		//	{
		//		if (hardDriveIDArrayElement.IsString() == true)
		//		{
		//			ld.hardDriveID.push_back(hardDriveIDArrayElement.GetString());
		//		}
		//	}
		//}

		//auto const systemSerial = settings.FindMember("systemSerial");

		//if (systemSerial != settings.MemberEnd() && systemSerial->value.IsString())
		//{
		//	ld.systemSerial = systemSerial->value.GetString();
		//}

		//auto const systemName = settings.FindMember("systemName");

		//if (systemName != settings.MemberEnd() && systemName->value.IsString())
		//{
		//	ld.systemName = systemName->value.GetString();
		//}

		//auto const osName = settings.FindMember("osName");

		//if (osName != settings.MemberEnd() && osName->value.IsString())
		//{
		//	ld.osName = osName->value.GetString();
		//}

		//auto const osVersion = settings.FindMember("osVersion");

		//if (osVersion != settings.MemberEnd() && osVersion->value.IsString())
		//{
		//	ld.osVersion = osVersion->value.GetString();
		//}

		//auto const systemID = settings.FindMember("systemID");

		//if (systemID != settings.MemberEnd() && systemID->value.IsString())
		//{
		//	ld.systemID = systemID->value.GetString();
		//}
	}
	else
	{
		printf("Settings::ConvertJsonStrToData error: \"settings\" field not found in string.\n"); 
		return false;
	}


	//if (document.HasMember("requested") == true)
	//{
	//	const rapidjson::Value& requested = document["requested"];

	//	auto const trialMode = requested.FindMember("trialMode");

	//	if (trialMode != requested.MemberEnd() && trialMode->value.IsBool())
	//	{
	//		ld.requested.trialMode = trialMode->value.GetBool();
	//	}

	//	auto const rentalMode = requested.FindMember("rentalMode");

	//	if (rentalMode != requested.MemberEnd() && rentalMode->value.IsBool())
	//	{
	//		ld.requested.rentalMode = rentalMode->value.GetBool();
	//	}

	//	auto const erased = requested.FindMember("erased");

	//	if (erased != requested.MemberEnd() && erased->value.IsBool())
	//	{
	//		ld.requested.erased = erased->value.GetBool();
	//	}

	//	auto const numCameras = requested.FindMember("numCameras");

	//	if (numCameras != requested.MemberEnd() && numCameras->value.IsInt())
	//	{
	//		ld.requested.numCameras = numCameras->value.GetInt();
	//	}

	//	auto const unusualBehaviours = requested.FindMember("unusualBehaviours");

	//	if (unusualBehaviours != requested.MemberEnd() && unusualBehaviours->value.IsInt())
	//	{
	//		ld.requested.unusualBehaviours = unusualBehaviours->value.GetInt();
	//	}

	//	auto const tripWires = requested.FindMember("tripWires");

	//	if (tripWires != requested.MemberEnd() && tripWires->value.IsInt())
	//	{
	//		ld.requested.tripWires = tripWires->value.GetInt();
	//	}

	//	auto const leftObjects = requested.FindMember("leftObjects");

	//	if (leftObjects != requested.MemberEnd() && leftObjects->value.IsInt())
	//	{
	//		ld.requested.leftObjects = leftObjects->value.GetInt();
	//	}

	//	auto const trexTier1s = requested.FindMember("trexTier1s");

	//	if (trexTier1s != requested.MemberEnd() && trexTier1s->value.IsInt())
	//	{
	//		ld.requested.trexTier1s = trexTier1s->value.GetInt();
	//	}

	//	auto const trexTier2s = requested.FindMember("trexTier2s");

	//	if (trexTier2s != requested.MemberEnd() && trexTier2s->value.IsInt())
	//	{
	//		ld.requested.trexTier2s = trexTier2s->value.GetInt();
	//	}

	//	auto const deFences = requested.FindMember("deFences");

	//	if (deFences != requested.MemberEnd() && deFences->value.IsInt())
	//	{
	//		ld.requested.deFences = deFences->value.GetInt();
	//	}

	//	auto const loiterings = requested.FindMember("loiterings");

	//	if (loiterings != requested.MemberEnd() && loiterings->value.IsInt())
	//	{
	//		ld.requested.loiterings = loiterings->value.GetInt();
	//	}

	//	auto const deepLearnings = requested.FindMember("deepLearnings");

	//	if (deepLearnings != requested.MemberEnd() && deepLearnings->value.IsInt())
	//	{
	//		ld.requested.deepLearnings = deepLearnings->value.GetInt();
	//	}
	//}
	//else
	//{
	//	printf("LicensingV2::ConvertJsonStrToLicenseData error: \"requested\" field not found in string.\n"); 
	//	return false;
	//}


	return true;
}

bool Settings::ConvertDataToJsonStr(JsonStr& jsonStr)
{
	rapidjson::StringBuffer jsonStringBuffer;
   rapidjson::Writer<rapidjson::StringBuffer> writer;

   jsonStringBuffer.Clear();
   writer.Reset(jsonStringBuffer);

   writer.StartObject(); // initial {
		writer.Key("settings");
	   writer.StartObject(); // settings
			//writer.Key("userID");
			//writer.String(ld.userID.c_str());

			//writer.Key("siteID");
			//writer.String(ld.siteID.c_str());

			//writer.Key("siteDescription");
			//writer.String(ld.siteDescription.c_str());

			//writer.Key("reSellerID");
			//writer.String(ld.reSellerID.c_str());

			//writer.Key("requestID");
			//writer.String(ld.requestID.c_str());

			//writer.Key("productID");
			//writer.String(ld.productID.c_str());

			//writer.Key("versionID");
			//writer.String(ld.versionID.c_str());

			//writer.Key("cpuID");
		 //  writer.StartArray();
			//for (int i = 0; i < ld.cpuID.size(); i++)
			//{
			//	writer.String(ld.cpuID[i].c_str());
			//}
		 //  writer.EndArray();

			//writer.Key("gpuID");
		 //  writer.StartArray();
			//for (int i = 0; i < ld.gpuID.size(); i++)
			//{
			//	writer.String(ld.gpuID[i].c_str());
			//}
		 //  writer.EndArray();

			//writer.Key("networkdID");
		 //  writer.StartArray();
			//for (int i = 0; i < ld.networkdID.size(); i++)
			//{
			//	writer.String(ld.networkdID[i].c_str());
			//}
		 //  writer.EndArray();

			//writer.Key("hardDriveID");
		 //  writer.StartArray();
			//for (int i = 0; i < ld.hardDriveID.size(); i++)
			//{
			//	writer.String(ld.hardDriveID[i].c_str());
			//}
		 //  writer.EndArray();

			//writer.Key("systemSerial");
			//writer.String(ld.systemSerial.c_str());

			//writer.Key("systemName");
			//writer.String(ld.systemName.c_str());

			//writer.Key("osName");
			//writer.String(ld.osName.c_str());

			//writer.Key("osVersion");
			//writer.String(ld.osVersion.c_str());

			//writer.Key("systemID");
			//writer.String(ld.systemID.c_str());
	   writer.EndObject(); // settings

		//writer.Key("requested");
	 //  writer.StartObject(); // requested
		//	writer.Key("trialMode");
		//	writer.Bool(ld.requested.trialMode);

		//	writer.Key("rentalMode");
		//	writer.Bool(ld.requested.rentalMode);

		//	writer.Key("erased");
		//	writer.Bool(ld.requested.erased);

		//	writer.Key("numCameras");
		//	writer.Int(ld.requested.numCameras);

		//	writer.Key("unusualBehaviours");
		//	writer.Int(ld.requested.unusualBehaviours);

		//	writer.Key("tripWires");
		//	writer.Int(ld.requested.tripWires);

		//	writer.Key("leftObjects");
		//	writer.Int(ld.requested.leftObjects);

		//	writer.Key("trexTier1s");
		//	writer.Int(ld.requested.trexTier1s);

		//	writer.Key("trexTier2s");
		//	writer.Int(ld.requested.trexTier2s);

		//	writer.Key("deFences");
		//	writer.Int(ld.requested.deFences);

		//	writer.Key("loiterings");
		//	writer.Int(ld.requested.loiterings);

		//	writer.Key("deepLearnings");
		//	writer.Int(ld.requested.deepLearnings);
	 //  writer.EndObject(); // requested
   writer.EndObject(); // final }

	jsonStr = jsonStringBuffer.GetString();

	return true;
}

std::string Settings::LoadFileToString(const std::string& filename)
{
   std::ifstream ifs(filename); // taking file as inputstream
   std::string str;

   if (ifs)
	{
      std::ostringstream ss;
      ss << ifs.rdbuf(); // reading data
      str = ss.str();
		ifs.close();
   }

	return str;
}

bool Settings::SaveStringToFile(const std::string& str, const std::string& filename)
{
	std::ofstream out(filename);

	if (out.fail() == true)
	{
		return false;
	}

	out << str;
	out.close();

	return true;
}


