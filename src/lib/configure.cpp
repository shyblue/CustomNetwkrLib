#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "util/configure.h"
#include "util/logger.h"

ConfigureData::ConfigureData(void)
{
}


ConfigureData::~ConfigureData(void)
{
}

bool ConfigureData::Initialize(const std::string ConfigureFilePath)
{
    try
    {
        LoadCofigureFile(ConfigureFilePath);
    }
    catch(std::exception& exception)
    {
        ST_LOGGER.Error(L"[ConfigureData] Initialize Error[%s]", exception.what());
        return false;
    }

    return true;
}

bool ConfigureData::LoadCofigureFile( const std::string ConfigureFilePath)
{


    // 해당위치에서 Configure 파일을 읽는다.
    boost::filesystem::path path(boost::filesystem::initial_path());
    path = ConfigureFilePath.c_str();
    if(!boost::filesystem::exists(boost::filesystem::path(path) ) )
    {
        // ERR_CONFIGURE_NOTPATH  Error 기록
        ST_LOGGER.Error("[ConfigureData][LoadCofigureFile] Not exist File[%s]", path.c_str() );
        return false;
    }

    m_configurePath = ConfigureFilePath;
    boost::property_tree::ini_parser::read_ini(path.string().c_str(), m_iniTree);

    OutConfigureData();

    return true;
}

bool ConfigureData::WriteConfigureFile()
{
    boost::property_tree::ini_parser::write_ini(m_configurePath.c_str(), m_iniTree);
    return true;
}

void ConfigureData::OutConfigureData()
{
    ST_LOGGER.Trace("[PRINT CONFIGURE DATA START]");
    for(auto group : m_iniTree)
    {
        std::string group_nm = group.first;
        for (auto key_value : group.second)
        {
            ST_LOGGER.Trace("[%s][%s][%s = %s]",m_configurePath.c_str(),group_nm.c_str(),key_value.first.c_str(), key_value.second.data().c_str());
        }
    }
	ST_LOGGER.Trace("[PRINT CONFIGURE DATA END]");
}

void ConfigureData::SetConfigureData(std::string key, uint32_t value)
{
    m_iniTree.erase(key);
    m_iniTree.put(key, value);
}

void ConfigureData::SetConfigureData(std::string key, std::string value)
{
    m_iniTree.erase(key);
    m_iniTree.put(key, value);
}
