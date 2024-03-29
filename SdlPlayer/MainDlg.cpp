// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
#include <helper/SFunctor.hpp>

#define SUBTITLE_TIMER 100

typedef transvod_ns::ITransVodPtr (*funCreateTransVod)(const char *model,
													   const char *sysver,
													   uint32_t playerContextId,
													   const char *cacheDir,
													   transvod_ns::ITransVodListener *pListener);
typedef void (*funTransVod_GetVersion)(char* pBuf, int nBufLen);
typedef void (*funTransVod_FreeSingletons)();
typedef BOOL (*funTransVod_IsSupportMedia)(const char* pszUrl,int *errCode);

funTransVod_IsSupportMedia g_fun_IsSupportMedia=NULL;


CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND")),m_hVodPlayer(0)
{
}

CMainDlg::~CMainDlg()
{
}


void CMainDlg::initLibrary(void)
{
	if (m_hVodPlayer) {
		return;
	}

	m_hVodPlayer = LoadLibrary(_T("vodplayer.dll"));
	if (m_hVodPlayer) {
		funCreateTransVod funCreate = (funCreateTransVod)GetProcAddress(m_hVodPlayer, "CreateTransVod");
		funTransVod_GetVersion funVer = (funTransVod_GetVersion)GetProcAddress(m_hVodPlayer, "TransVod_GetVersion");
		g_fun_IsSupportMedia = (funTransVod_IsSupportMedia)GetProcAddress(m_hVodPlayer, "TransVod_IsSupportMedia");
		if (funCreate) {
			m_vodPlayer.Attach(funCreate("windows", "10.0", 34567, "d:\\vodCache", this));
			m_vodPlayer->setQualityReportInterval(10000);

			FILE* f = fopen("hardware.json", "rb");
			if (f)
			{
				char szConfig[500] = { 0 };
				fread(szConfig, 1, 500, f);
				m_vodPlayer->setHardwareDocoderConfig(szConfig);
				fclose(f);
			}
			else
			{
				m_vodPlayer->setHardwareDocoderConfig("{\"min_height\":360,\"enable_hardware_decode\":false}");
			}
		}
	}
	else {
		WCHAR szBuf[100];
		swprintf_s(szBuf, 100, L"load vod player failed,code=%d", GetLastError());
		MessageBox(m_hWnd,szBuf, L"error", MB_OK);
	}
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	initLibrary();

	return 0;
}


void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if(m_render)
		m_render->rerender();
}

void CMainDlg::onP2PJsonContent(THIS_ int32_t cbKey, const char* json)
{

}

void CMainDlg::onP2PShareStats(THIS_ int32_t shareUpStreamFlow, int32_t shareDownStreamFlow, int32_t serverDownStreamFlow)
{

}

void CMainDlg::onBitrateReport(THIS_ uint32_t audioBitrate,uint32_t videoBitrate)
{

}

void CMainDlg::onQualityReport(THIS_ uint32_t alt, uint32_t rtf, uint32_t rvf, uint32_t rvff, uint32_t rcf, uint32_t rcff)
{

}

void CMainDlg::onMediaInfo(THIS_ LPCSTR url, BOOL hasAudio, BOOL hasVideo)
{
	SStringA strUrl(url);
	STaskHelper::post(GetMsgLoop(),this,&CMainDlg::_onMediaInfo,strUrl,hasAudio,hasVideo);
}

void CMainDlg::onPlayerNetRequestStatus(THIS_ LPCSTR url, int32_t status, int32_t networkQuality, LPCSTR svrIp, BOOL bConnnected, int32_t nhttpCode)
{

}

void CMainDlg::onVideoCacheCompleted(THIS_ LPCSTR url, LPCSTR pszCachePath)
{

}

void CMainDlg::onPlayedEndOneLoop(THIS_ LPCSTR url)
{

}

void CMainDlg::onPlayedSeekFinish(THIS_ LPCSTR url, uint32_t seekTime)
{

}

void CMainDlg::onPlayerSeekStart (THIS_ LPCSTR url, uint32_t seekTime, uint64_t seekId)
{

}

void CMainDlg::onResolution (THIS_ LPCSTR url, uint32_t wid, uint32_t hei)
{

}

void CMainDlg::onCatonTimes(THIS_ LPCSTR url, uint32_t * catonTimes, int nSize)
{

}

void CMainDlg::onDownloadSpeed(THIS_ LPCSTR url, uint32_t costTime, uint32_t speed, BOOL isComplete)
{

}

void CMainDlg::onError(THIS_ LPCSTR url, transvod::ErrorCode errCode, int statusCode)
{
	SStringA strUrl(url);
	STaskHelper::post(GetMsgLoop(),this,&CMainDlg::_onError,strUrl,errCode,statusCode);
}

void CMainDlg::_onError(const SStringA & url, transvod::ErrorCode errCode, int statusCode)
{
	OnStop();
}

void CMainDlg::onStatics(THIS_ LPCSTR url, transvod::PlayerStaticsType type, LPCSTR data, uint32_t len)
{

}

void CMainDlg::onResourceTotalSize(THIS_ LPCSTR url, uint32_t totalSize)
{

}

void CMainDlg::onCacheTimeChanged(THIS_ LPCSTR url, uint32_t cacheTime)
{

}

void CMainDlg::onPlayedTimeChanged(THIS_ LPCSTR url, uint32_t playedTime)
{
	SStringA strUrl(url);
	STaskHelper::post(GetMsgLoop(),this,&CMainDlg::_onPlayedTimeChanged,strUrl,playedTime);
}

void CMainDlg::onLoadingChanged(THIS_ LPCSTR url, uint32_t percent)
{

}

void CMainDlg::onTotalTime(THIS_ LPCSTR url, uint32_t totalTime)
{
	SStringA strUrl(url);
	STaskHelper::post(GetMsgLoop(),this,&CMainDlg::_onTotalTime,strUrl,totalTime);
}

void CMainDlg::onStateChanged(THIS_ LPCSTR url, transvod::PlayerState state, transvod::ErrorReason reason)
{

}

void CMainDlg::onAudioFrame(THIS_ LPCSTR url, IAVframe *frame)
{

}

void CMainDlg::onVideoFrame(THIS_ LPCSTR url, IAVframe *frame)
{
	SStringA strUrl(url);
	SComPtr<IAVframe> frm(frame);
	STaskHelper::post(GetMsgLoop(),this,&CMainDlg::_onVideoFrame,strUrl,frm);
}

BOOL CMainDlg::onGetAudioDecoderParam(THIS_ AudioDecoderParam *pParam)
{
	return FALSE;
}

BOOL CMainDlg::onGetVideoDecoderParam(THIS_ VideoDecoderParam *pParam)
{
	return FALSE;
}

void CMainDlg::onVideoFormatChanged(THIS_ LPCSTR url, const VideoInfo *mediaInfo)
{

}

void CMainDlg::onAudioFormatChanged(THIS_ LPCSTR url, const AudioInfo *mediaInfo)
{

}

void CMainDlg::onAVStream(THIS_ LPCSTR url, const IAVStream **streams, int nStreamCount, const char* metaData, BOOL isLastFrame)
{

}

#include <core/types/UrlProtocol.h>
using namespace transvod;
void CMainDlg::OnPlay()
{
	SEdit *pEdit = FindChildByID2<SEdit>(R.id.edit_url);
	SStringT strUrl = pEdit->GetWindowText();
	if(m_vodPlayer)
	{
		FindChildByID(R.id.sdl_back)->SetVisible(FALSE);
		FindChildByID(R.id.sdl_front)->SetVisible(TRUE);
		SSdlView *pSdlView = FindChildByID2<SSdlView>(R.id.sdl_view);
		SStringA urlUtf8 = S_CT2A(strUrl,CP_UTF8);

		m_render.Attach(new SDL2BuiltinRender(m_vodPlayer,pSdlView,VIDEO_RENDER_MODE_ASPECT_FIT));

		AudioDeviceParam param = { DEV_UTF8_NAME, "" };
		m_vodPlayer->setAudioBuiltinRender(&param,1.0);

		int playTaskId = 100;  // any value.
		SStringA url2 = urlUtf8;
		url2.MakeLower();
		BOOL bFlv = url2.Find("flv") != -1 && url2.Right(3) != "mp4";

		BOOL bUrl = (url2.Left(4).CompareNoCase("http") == 0);
		if (!bUrl)
		{//convert to utf8
			int code = 0;
			if (g_fun_IsSupportMedia(urlUtf8, &code) == FALSE) {
				SMessageBox(m_hWnd,L"not support format type.", L"error", MB_OK);
			} else {
				m_vodPlayer->play(
					urlUtf8, transvod::UrlProtocol_File,
					transvod::DataSourceFormat_MP4,
					transvod::CachePolicy_NoCache, playTaskId, FALSE, 0, NULL);
			}
		} else {
			if (bFlv)
			{
				m_vodPlayer->play(
					urlUtf8,  transvod::UrlProtocol_Http,
					transvod::DataSourceFormat_FLV,
					transvod::CachePolicy_NoCache, playTaskId, TRUE, 0, NULL);
			}
			else
			{
				m_vodPlayer->play(
					urlUtf8, transvod::UrlProtocol_Http,
					transvod::DataSourceFormat_MP4,
					transvod::CachePolicy_Whole_File_Content, playTaskId, FALSE, 0, NULL);
			}
		}
	}
}

void CMainDlg::OnStop()
{
	SNativeWnd::KillTimer(SUBTITLE_TIMER);
	m_render = NULL;
	if(m_vodPlayer)
	{
		m_vodPlayer->stop(TRUE);
	}
	FindChildByID(R.id.sdl_back)->SetVisible(TRUE,TRUE);
	FindChildByID(R.id.sdl_front)->SetVisible(FALSE,TRUE);
}


void CMainDlg::OnDestory()
{
	m_render = NULL;
	m_vodPlayer->stop(TRUE);
	m_vodPlayer = NULL;

	SetMsgHandled(FALSE);
}

void CMainDlg::_onTotalTime(const SStringA &url,uint32_t totalTime)
{
	SSliderBar *pSlider = FindChildByID2<SSliderBar>(R.id.slider_prog);
	SWindow *pTxt = FindChildByID(R.id.txt_duration);

	if(totalTime!=UINT_MAX/2)
	{
		pSlider->SetRange(0,totalTime);
		pSlider->SetValue(0);

		uint32_t ms = totalTime % 1000;
		totalTime/=1000;
		uint32_t sec = totalTime % 60;
		totalTime/=60;
		uint32_t minute = totalTime%60;
		totalTime/=60;
		uint32_t hour = totalTime;
		pSlider->EnableWindow(TRUE);
		pTxt->SetWindowText(SStringT().Format(_T("%u:%u:%u"),hour,minute,sec));
	}else
	{
		pSlider->EnableWindow(FALSE);
		pTxt->SetWindowText(_T("--:--"));
	}

}

void CMainDlg::_onPlayedTimeChanged(const SStringA &url,uint32_t playedTime)
{
	SSliderBar *pSlider = FindChildByID2<SSliderBar>(R.id.slider_prog);
	if(pSlider) pSlider->SetValue(playedTime);
}

void CMainDlg::OnSliderPos(IEvtArgs *evt)
{
	EventSliderPos *e2=sobj_cast<EventSliderPos>(evt);
	m_vodPlayer->seekTo(e2->nPos);
}

void CMainDlg::_onVideoFrame(const SStringA &url, SComPtr<IAVframe> frame)
{
	if(m_render)
	{
		m_render->onRenderFrame(frame);
	}
}

void CMainDlg::_onMediaInfo(const SStringA & url, BOOL hasAudio, BOOL hasVideo)
{
	SNativeWnd::SetTimer(SUBTITLE_TIMER,100,NULL);
}

void CMainDlg::OnTimer(UINT_PTR idEvent)
{
	if(idEvent == SUBTITLE_TIMER)
	{
		SScrollSubtitles *pSubTitle = FindChildByName2<SScrollSubtitles>("scroll_subtitles");
		if(pSubTitle)
		{
			const LPCTSTR says[]={
				_T("SOUI4很强大"),
				_T("全部核心对象导致类COM接口"),
				_T("方便C语言调用"),
				_T("QQ群：229313785"),
				_T("https://github.com/soui4/soui"),
			};
			int num = ARRAYSIZE(says);
			pSubTitle->AddSubtitles(says[rand()%num],rand()%4);
			SNativeWnd::KillTimer(idEvent);
			SNativeWnd::SetTimer(idEvent,rand()%100,NULL);
		}
	}else
	{
		SetMsgHandled(FALSE);
	}
}

