#include "weather.hpp"

#include "../mwrender/renderingmanager.hpp"

using namespace Ogre;
using namespace MWWorld;

#define TRANSITION_TIME 10

WeatherManager::WeatherManager(MWRender::RenderingManager* rendering) : 
    mCurrentWeather("clear")
{
    mRendering = rendering;
    
    #define clr(r,g,b) ColourValue(r/255.f, g/255.f, b/255.f)
    
    /// \todo read these from Morrowind.ini
    Weather clear;
    clear.mCloudTexture = "tx_sky_clear.dds";
    clear.mCloudsMaximumPercent = 1.0;
    clear.mTransitionDelta = 0.15;
    clear.mSkySunriseColor = clr(118, 141, 164);
    clear.mSkyDayColor = clr(95, 135, 203);
    clear.mSkySunsetColor = clr(56, 89, 129);
    clear.mSkyNightColor = clr(9, 10, 11);
    clear.mFogSunriseColor = clr(255, 189, 157);
    clear.mFogDayColor = clr(206, 227, 255);
    clear.mFogSunsetColor = clr(255, 189, 157);
    clear.mFogNightColor = clr(9, 10, 11);
    clear.mAmbientSunriseColor = clr(47, 66, 96);
    clear.mAmbientDayColor = clr(137, 140, 160);
    clear.mAmbientSunsetColor = clr(68, 75, 96);
    clear.mAmbientNightColor = clr(32, 35, 42);
    clear.mSunSunriseColor = clr(242, 159, 99);
    clear.mSunDayColor = clr(255, 252, 238);
    clear.mSunSunsetColor = clr(255, 115, 79);
    clear.mSunNightColor = clr(59, 97, 176);
    clear.mSunDiscSunsetColour = clr(255, 189, 157);
    clear.mLandFogDayDepth = 0.69;
    clear.mLandFogNightDepth = 0.69;
    clear.mWindSpeed = 0.1;
    clear.mCloudSpeed = 1.25;
    clear.mGlareView = 1.0;
    
    mWeatherSettings["clear"] = clear;
    
    Weather cloudy;
    cloudy.mCloudTexture = "tx_sky_cloudy.dds";
    cloudy.mCloudsMaximumPercent = 1.0;
    cloudy.mTransitionDelta = 0.15;
    cloudy.mSkySunriseColor = clr(126, 158, 173);
    cloudy.mSkyDayColor = clr(117, 160, 215);
    cloudy.mSkySunsetColor = clr(111, 114, 159);
    cloudy.mSkyNightColor = clr(9, 10, 11);
    cloudy.mFogSunriseColor = clr(255, 207, 149);
    cloudy.mFogDayColor = clr(245, 235, 224);
    cloudy.mFogSunsetColor = clr(255, 155, 106);
    cloudy.mFogNightColor = clr(9, 10, 11);
    cloudy.mAmbientSunriseColor = clr(66, 74, 87);
    cloudy.mAmbientDayColor = clr(137, 145, 160);
    cloudy.mAmbientSunsetColor = clr(71, 80, 92);
    cloudy.mAmbientNightColor = clr(32, 39, 54);
    cloudy.mSunSunriseColor = clr(241, 177, 99);
    cloudy.mSunDayColor = clr(255, 236, 221);
    cloudy.mSunSunsetColor = clr(255, 89, 00);
    cloudy.mSunNightColor = clr(77, 91, 124);
    cloudy.mSunDiscSunsetColour = clr(255, 202, 179);
    cloudy.mLandFogDayDepth = 0.72;
    cloudy.mLandFogNightDepth = 0.72;
    cloudy.mWindSpeed = 0.2;
    cloudy.mCloudSpeed = 2;
    cloudy.mGlareView = 1.0;
    
    mWeatherSettings["cloudy"] = cloudy;
    
    /*
    Weather overcast;
    overcast.mCloudTexture = "tx_sky_overcast.dds";
    overcast.mCloudsMaximumPercent = 1.0;
    mWeatherSettings["overcast"] = overcast;
    */
    
    setWeather("clear", true);
    setWeather("cloudy", false);
}

void WeatherManager::setWeather(const String& weather, bool instant)
{
    if (instant)
    {
        mNextWeather = "";
        mCurrentWeather = weather;
    }
    else if (weather != mCurrentWeather)
    {
        mNextWeather = weather;
        mRemainingTransitionTime = TRANSITION_TIME;
    }
}

WeatherResult WeatherManager::getResult(const String& weather)
{
    const Weather& current = mWeatherSettings[weather];
    WeatherResult result;
    
    result.mCloudTexture = current.mCloudTexture;
    result.mCloudBlendFactor = 0;
    result.mCloudOpacity = current.mCloudsMaximumPercent;
    /// \todo
    
    return result;
}

WeatherResult WeatherManager::transition(float factor)
{
    const WeatherResult& current = getResult(mCurrentWeather);
    const WeatherResult& other = getResult(mNextWeather);
    WeatherResult result;
    
    result.mCloudTexture = current.mCloudTexture;
    result.mNextCloudTexture = other.mCloudTexture;
    result.mCloudBlendFactor = factor;
        
    #define lerp(x, y) (x * (1-factor) + y * factor)
    
    result.mCloudOpacity = lerp(current.mCloudOpacity, other.mCloudOpacity);
    
    /// \todo
    
    return result;
}

void WeatherManager::update(float duration)
{
    WeatherResult result;
    
    if (mNextWeather != "")
    {
        mRemainingTransitionTime -= duration;
        if (mRemainingTransitionTime < 0)
        {
            mCurrentWeather = mNextWeather;
            mNextWeather = "";
        }
    }
    
    if (mNextWeather != "")
        result = transition(1-(mRemainingTransitionTime/TRANSITION_TIME));
    else
        result = getResult(mCurrentWeather);
    
    mRendering->getSkyManager()->setWeather(result);
}

void WeatherManager::setHour(const float hour)
{
    mHour = hour;
}

void WeatherManager::setDate(const int day, const int month)
{
    mDay = day;
    mMonth = month;
}
