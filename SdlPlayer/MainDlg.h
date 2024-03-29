// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <transvod-i.h>
#include "VideoBuiltinRender.h"

using namespace transvod_ns;

class CMainDlg : public SHostWnd , public ITransVodListener
{
public:
	CMainDlg();
	~CMainDlg();

protected:
	HMODULE m_hVodPlayer;
	SComPtr<ITransVod> m_vodPlayer;
	SComPtr<SDL2BuiltinRender> m_render;
	void initLibrary(void);
protected:
	STDMETHOD_(void, onAVStream)(THIS_ LPCSTR url, const IAVStream **streams, int nStreamCount, const char* metaData, BOOL isLastFrame) override;

	STDMETHOD_(void, onAudioFormatChanged)(THIS_ LPCSTR url, const AudioInfo *mediaInfo) override;

	STDMETHOD_(void, onVideoFormatChanged)(THIS_ LPCSTR url, const VideoInfo *mediaInfo) override;    

	STDMETHOD_(BOOL, onGetVideoDecoderParam)(THIS_ VideoDecoderParam *pParam) override;

	STDMETHOD_(BOOL, onGetAudioDecoderParam)(THIS_ AudioDecoderParam *pParam) override;

	STDMETHOD_(void, onVideoFrame)(THIS_ LPCSTR url, IAVframe *frame) override;
	STDMETHOD_(void, onAudioFrame)(THIS_ LPCSTR url, IAVframe *frame) override;

	STDMETHOD_(void, onStateChanged)(THIS_ LPCSTR url, transvod::PlayerState state, transvod::ErrorReason reason) override;

	STDMETHOD_(void, onTotalTime)(THIS_ LPCSTR url, uint32_t totalTime) override;

	STDMETHOD_(void, onLoadingChanged)(THIS_ LPCSTR url, uint32_t percent) override;

	STDMETHOD_(void, onPlayedTimeChanged)(THIS_ LPCSTR url, uint32_t playedTime) override;

	STDMETHOD_(void, onCacheTimeChanged)(THIS_ LPCSTR url, uint32_t cacheTime) override;

	STDMETHOD_(void, onResourceTotalSize)(THIS_ LPCSTR url, uint32_t totalSize) override;

	STDMETHOD_(void, onStatics)(THIS_ LPCSTR url, transvod::PlayerStaticsType type, LPCSTR data, uint32_t len) override;

	STDMETHOD_(void, onError)(THIS_ LPCSTR url, transvod::ErrorCode errCode, int statusCode) override;

	STDMETHOD_(void, onDownloadSpeed)(THIS_ LPCSTR url, uint32_t costTime, uint32_t speed, BOOL isComplete) override;

	STDMETHOD_(void, onCatonTimes)(THIS_ LPCSTR url, uint32_t * catonTimes, int nSize) override;

	STDMETHOD_(void, onResolution)(THIS_ LPCSTR url, uint32_t wid, uint32_t hei) override;

	STDMETHOD_(void, onPlayerSeekStart)(THIS_ LPCSTR url, uint32_t seekTime, uint64_t seekId) override;

	STDMETHOD_(void, onPlayedSeekFinish)
		(THIS_ LPCSTR url, uint32_t seekTime) override;

	STDMETHOD_(void, onPlayedEndOneLoop)(THIS_ LPCSTR url) override;

	STDMETHOD_(void, onVideoCacheCompleted)(THIS_ LPCSTR url, LPCSTR pszCachePath) override;

	STDMETHOD_(void, onPlayerNetRequestStatus)(THIS_ LPCSTR url, int32_t status, int32_t networkQuality, LPCSTR svrIp, BOOL bConnnected, int32_t nhttpCode) override;

	STDMETHOD_(void, onMediaInfo)(THIS_ LPCSTR url, BOOL hasAudio, BOOL hasVideo) override;

	STDMETHOD_(void, onQualityReport)
		(THIS_ uint32_t alt, uint32_t rtf, uint32_t rvf, uint32_t rvff, uint32_t rcf, uint32_t rcff) override;
	STDMETHOD_(void, onBitrateReport)
		(THIS_ uint32_t audioBitrate,uint32_t videoBitrate) override;

	STDMETHOD_(void,onP2PShareStats)(THIS_ int32_t shareUpStreamFlow, int32_t shareDownStreamFlow, int32_t serverDownStreamFlow) override;
	STDMETHOD_(void,onP2PJsonContent)(THIS_ int32_t cbKey, const char* json) override;

protected:
	void _onTotalTime(const SStringA &url,uint32_t totalTime);
	void _onPlayedTimeChanged(const SStringA &url,uint32_t playedTime);
	void _onVideoFrame(const SStringA &url, SComPtr<IAVframe> frame);
	void _onError(const SStringA & url, transvod::ErrorCode errCode, int statusCode);
	void _onMediaInfo(const SStringA & url, BOOL hasAudio, BOOL hasVideo);

protected:
	void OnPlay();
	void OnStop();
	void OnSliderPos(IEvtArgs *evt);
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_ID_COMMAND(R.id.btn_play,OnPlay)
		EVENT_ID_COMMAND(R.id.btn_stop,OnStop)
		EVENT_ID_HANDLER(R.id.slider_prog,EventSliderPos::EventID,OnSliderPos)
	EVENT_MAP_END()
		
protected:
	void OnSize(UINT nType, CSize size);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnDestory();
	void OnTimer(UINT_PTR idEvent);
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestory)
		MSG_WM_SIZE(OnSize)
		MSG_WM_TIMER(OnTimer)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
};
