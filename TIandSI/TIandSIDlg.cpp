/**
 * TIandSI
 *
 * ������ Lei Xiaohua ���� Zhang Hui
 * leixiaohua1020@126.com
 * zhanghuicuc@gmail.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ���������ڼ�����Ƶ��ʱ����Ϣ��TI��Ҳ��Ϊʱ�临�Ӷȣ����Լ��ռ�
 * ��Ϣ��SI��Ҳ��Ϊ�ռ临�Ӷȣ�����������ָ��ȡ����ITU-R BT.1788��׼��
 * 
 * This software is used to calculate raw video's TI (Temporal perceptual 
 * Information) and SI (Spatial perceptual Information) defined in 
 * ITU-R BT.1788.
 */


#include "stdafx.h"
#include "TIandSI.h"
#include "TIandSIDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTIandSIDlg �Ի���




CTIandSIDlg::CTIandSIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTIandSIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_inputyuvheight = 0;
	m_inputyuvwidth = 0;
	//  m_radiodraw = 0;
	m_padding = 0;
}

void CTIandSIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YUVLIST, m_yuvlist);
	DDX_Control(pDX, IDC_YUVURL, m_yuvurl);
	DDX_Control(pDX, IDC_INPUT_YUV_PIXFMT, m_inputyuvpixfmt);
	DDX_Text(pDX, IDC_INPUT_YUV_HEIGHT, m_inputyuvheight);
	DDX_Text(pDX, IDC_INPUT_YUV_WIDTH, m_inputyuvwidth);
	DDX_Control(pDX, IDC_OUTPUTSI, m_outputsi);
	DDX_Control(pDX, IDC_OUTPUTTI, m_outputti);
	DDX_Control(pDX, IDC_OUTPUTSIYUV, m_outputsiyuv);
	DDX_Control(pDX, IDC_OUTPUTTIYUV, m_outputtiyuv);
	DDX_Control(pDX, IDC_PROGRESS_ALL, m_progressall);
	DDX_Control(pDX, IDC_PROGRESS_ALL_TEXT, m_progressalltext);
	DDX_Control(pDX, IDC_PROGRESS_CUR_TI, m_progresscurti);
	DDX_Control(pDX, IDC_PROGRESS_CUR_SI, m_progresscursi);
	DDX_Control(pDX, IDC_PROGRESS_CUR_SI_TEXT, m_progresscursitext);
	DDX_Control(pDX, IDC_PROGRESS_CUR_TI_TEXT, m_progresscurtitext);
	DDX_Control(pDX, IDC_CURVAL, m_curval);
	DDX_Control(pDX, IDC_COMBO_DRAW, m_combodraw);
	DDX_Text(pDX, IDC_PADDING, m_padding);
}

BEGIN_MESSAGE_MAP(CTIandSIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTIandSIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_YUVLIST_ADD, &CTIandSIDlg::OnBnClickedYuvlistAdd)
	ON_BN_CLICKED(IDC_YUVLIST_DELETE, &CTIandSIDlg::OnBnClickedYuvlistDelete)
	ON_BN_CLICKED(IDC_YUVLIST_DELETEALL, &CTIandSIDlg::OnBnClickedYuvlistDeleteall)
	ON_BN_CLICKED(IDABOUT, &CTIandSIDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDPAUSE, &CTIandSIDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDSTOP, &CTIandSIDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDRESULT, &CTIandSIDlg::OnBnClickedResult)
	ON_WM_DROPFILES()
	ON_COMMAND(IDADDDLG, &CTIandSIDlg::OnAdddlg)
	ON_COMMAND(IDWEBSITE, &CTIandSIDlg::OnWebsite)
END_MESSAGE_MAP()


// CTIandSIDlg ��Ϣ�������

BOOL CTIandSIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	//---------------
	m_inputyuvwidth=1920;
	m_inputyuvheight=1080;
	m_padding=1;
	m_inputyuvpixfmt.InsertString(0,L"YUV420P");
	m_inputyuvpixfmt.InsertString(1,L"YUV422P");
	m_inputyuvpixfmt.InsertString(2,L"YUV444P");
	m_inputyuvpixfmt.InsertString(3,L"Y");
	m_inputyuvpixfmt.SetCurSel(0);
	
	m_outputsi.SetCheck(TRUE);
	m_outputsiyuv.SetCheck(TRUE);
	m_outputti.SetCheck(TRUE);
	m_outputtiyuv.SetCheck(TRUE);

	UpdateData(FALSE);
	//��ʼ��
	m_progressalltext.SetWindowText(L"0%");
	m_progresscurtitext.SetWindowText(L"0%");
	m_progresscursitext.SetWindowText(L"0%");
	m_progressall.SetRange(0,100);
	m_progresscurti.SetRange(0,100);
	m_progresscursi.SetRange(0,100);

	//SDL==========================
	sdlparam.graphically==true;
	//SDL_putenv()����ǰ��
	char variable[256];   
	CWnd* pWnd = GetDlgItem(IDC_SCREEN);  //��ȡͼƬ�ؼ��Ĵ���ָ��   
	sprintf(variable,"SDL_WINDOWID=0x%1x",pWnd->GetSafeHwnd()); // ��ʽ���ַ���      
	SDL_putenv(variable); 

	if(SDL_Init(SDL_INIT_VIDEO)) {
		AfxMessageBox(L"Could not initialize SDL"); 
		return 0;
	} 


	CRect screenrect;
	GetDlgItem(IDC_SCREEN)->GetWindowRect(screenrect);
	sdlparam.screen = SDL_SetVideoMode(screenrect.Width(), screenrect.Height(), 0, 0);
	if(!sdlparam.screen) {  
		AfxMessageBox(L"SDL: could not set video mode");  
		return 0;
	}

	resultdlg=new ResultDlg;
	resultdlg->Create(IDD_RESULT_DIALOG);
	//-----------------------------
	m_combodraw.InsertString(0,L"��ʾ");
	m_combodraw.InsertString(1,L"����ʾ");
	m_combodraw.SetCurSel(0);

	SetState(SYSTEM_PREPARE);

	//=============================

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTIandSIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTIandSIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTIandSIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CTIandSIDlg::TIandSI(YUVInfo yuvinfo,LPVOID lparam)
{
	CTIandSIDlg *dlg=(CTIandSIDlg *)lparam;;
	//----------------
	CFile cfYUV;
	CFile cfSI;//һ��yuv���У�y������Ϊ8�ֽڵ�siֵ
	CFile cfTI;//һ��yuv���У�y������Ϊ8�ֽڵ�tiֵ
	CFile cfSIData;//һ��y���У�y������Ϊfloat�͵�siֵ
	CFile cfTIData;//һ��y���У�y������Ϊfloat�͵�tiֵ
	FILE* fInfoSI;
	FILE* fInfoTI;

	int nFrameNum;
	float nFrameSize;
	int nYSize=yuvinfo.h * yuvinfo.w;

	unsigned char *pFrame=(unsigned char*)malloc(nYSize);
	unsigned char *pNextFrame=(unsigned char*)malloc(nYSize);
	unsigned char *pFrame_0;
	unsigned char *pFrame_1;
	unsigned char *pFrame_2;
	unsigned char *pNextFrame_0;

	unsigned long file_length;

	switch (yuvinfo.pixfmt) {
	case 400: nFrameSize=nYSize; break;
	case 422: nFrameSize=nYSize*2; break;
	case 444: nFrameSize = nYSize*3; break;
	default :
	case 420: nFrameSize = nYSize*1.5; break;
	}

	cfYUV.Open(yuvinfo.url, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead); 
	nFrameNum = cfYUV.GetLength()/nFrameSize;

	char name[255];
	char name1[255];
	DWORD len = WideCharToMultiByte(CP_OEMCP,NULL,(LPCTSTR)yuvinfo.url,-1,NULL,0,NULL,FALSE);
	WideCharToMultiByte(CP_OEMCP,NULL,(LPCTSTR)yuvinfo.url,-1,name,len,NULL,FALSE);
	//�����ļ�
	if(m_outputsi.GetCheck()!=FALSE){
		sprintf(name1, "%s_SI.csv", name);
		fInfoSI = fopen(name1, "w+");
	}
	if(m_outputti.GetCheck()!=FALSE){
		sprintf(name1,"%s_TI.csv", name);
		fInfoTI = fopen(name1, "w+");
	}
	if(m_outputsiyuv.GetCheck()!=FALSE){
		cfSI.Open(yuvinfo.url + _T("_SI.yuv"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite); 
	}
	//cfSIData.Open(strPath + _T("_SIData.y"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);

	int realframe_size=(yuvinfo.w-2*yuvinfo.padding)*(yuvinfo.h-2*yuvinfo.padding);
	
	unsigned char *pSobelScreen=(unsigned char*)malloc(realframe_size);
	unsigned char *pDiffScreen=(unsigned char*)malloc(realframe_size);
	memset(pSobelScreen,0,realframe_size);
	memset(pDiffScreen,0,realframe_size);

	float *frame_sobel=(float*)malloc(realframe_size*sizeof(float));
	memset(frame_sobel,0.0f,realframe_size*sizeof(float));
	float avg_frame_sobel=0;
	float *SI=(float*)malloc(nFrameNum*sizeof(float));
	float avg_SI=0;float max_SI;
	int index=0;

	float *frame_diff=(float*)malloc(realframe_size*sizeof(float));
	float avg_frame_diff=0;
	float *TI=(float*)malloc((nFrameNum-1)*sizeof(float));
	float avg_TI=0;float max_TI;

	__m128 sobel_avg_sum=_mm_set1_ps(+0.0f);
	__m128 sobel_square_sum=_mm_set1_ps(+0.0f);
	__m128 diff_avg_sum=_mm_set1_ps(+0.0f);
	__m128 diff_square_sum=_mm_set1_ps(+0.0f);
	__m128 avg_sobel=_mm_set1_ps(+0.0f);
	__m128 avg_diff=_mm_set1_ps(+0.0f);
	
	int i,j,k,m;
	int pad_threshold=0;

	//Draw--------------
	sdlparam.bmp = SDL_CreateYUVOverlay(yuvinfo.w-2*yuvinfo.padding, yuvinfo.h-2*yuvinfo.padding,SDL_YV12_OVERLAY, sdlparam.screen); 
	//FIX
	dlg->m_progresscurti.SetPos(0);
	dlg->m_progresscurtitext.SetWindowText(L"0%");
	dlg->m_progresscursi.SetPos(0);
	dlg->m_progresscursitext.SetWindowText(L"0%");

	//UV����ͼ��
	unsigned char* NewUVBuffer= (unsigned char*)malloc(realframe_size/2);
	memset(NewUVBuffer, 0x80, realframe_size/2);

	/********************����SI***********************/
	for(k = 0;k < nFrameNum; k++)//���n֡��ѭ��n-1��
	{
		//Check State-----
		while(dlg->sysstate==SYSTEM_PAUSE){
			Sleep(1000);
		}
		if(dlg->sysstate==SYSTEM_PREPARE){
			return -1;
		}
		//----------------

		cfYUV.Seek(k*nFrameSize, CFile::begin);
		cfYUV.Read(pFrame, nYSize);
		pFrame_0=pFrame+yuvinfo.w*(yuvinfo.padding-1);
		pFrame_1=pFrame+yuvinfo.w*yuvinfo.padding;
		pFrame_2=pFrame+yuvinfo.w*(yuvinfo.padding+1);

		for(j = yuvinfo.padding; j < yuvinfo.h-yuvinfo.padding; j++)
		{
			for(i = yuvinfo.padding; i < yuvinfo.w-yuvinfo.padding; i+=4)
			{
				if(i+4>yuvinfo.w-yuvinfo.padding)
					pad_threshold=1;
				// load 16 components. (0~6 will be used)
				__m128i current_0 = _mm_unpacklo_epi8(_mm_loadu_si128((__m128i*)(pFrame_0+i-1)), _mm_setzero_si128());
				__m128i current_1 = _mm_unpacklo_epi8(_mm_loadu_si128((__m128i*)(pFrame_1+i-1)), _mm_setzero_si128());
				__m128i current_2 = _mm_unpacklo_epi8(_mm_loadu_si128((__m128i*)(pFrame_2+i-1)), _mm_setzero_si128());
				
				// pFrame_00 = { pFrame_0[i-1], pFrame_0[i], pFrame_0[i+1], pFrame_0[i+2] }
				__m128 pFrame_00 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(current_0, _mm_setzero_si128()));
				// pFrame_01 = { pFrame_0[i], pFrame_0[i+1], pFrame_0[i+2], pFrame_0[i+3] }
				__m128 pFrame_01 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(_mm_srli_si128(current_0, 2), _mm_setzero_si128()));
				// pFrame_02 = { pFrame_0[i+1], pFrame_0[i+2], pFrame_0[i+3], pFrame_0[i+4] }
				__m128 pFrame_02 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(_mm_srli_si128(current_0, 4), _mm_setzero_si128()));
				// pFrame_10 = { pFrame_1[i-1], pFrame_1[i], pFrame_1[i+1], pFrame_1[i+2] }
				__m128 pFrame_10 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(current_1, _mm_setzero_si128()));
				// pFrame_12 = { pFrame_1[i+1], pFrame_1[i+2], pFrame_1[i+3], pFrame_1[i+4] }
				__m128 pFrame_12 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(_mm_srli_si128(current_1, 4), _mm_setzero_si128()));
				// pFrame_20 = { pFrame_2[i-1], pFrame_2[i], pFrame_2[i+1], pFrame_2[i+2] }
				__m128 pFrame_20 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(current_2, _mm_setzero_si128()));
				// pFrame_21 = { pFrame_2[i], pFrame_2[i+1], pFrame_2[i+2], pFrame_2[i+3] }
				__m128 pFrame_21 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(_mm_srli_si128(current_2, 2), _mm_setzero_si128()));
				// pFrame_22 = { pFrame_2[i+1], pFrame_2[i+2], pFrame_2[i+3], pFrame_2[i+4] }
				__m128 pFrame_22 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(_mm_srli_si128(current_2, 4), _mm_setzero_si128()));

				__m128 gx=_mm_add_ps(_mm_sub_ps(_mm_add_ps(_mm_add_ps(_mm_sub_ps(_mm_sub_ps(_mm_sub_ps(pFrame_20,pFrame_22),pFrame_12),pFrame_12),pFrame_10),pFrame_10),pFrame_02),pFrame_00);

				__m128 gy=_mm_sub_ps(_mm_sub_ps(_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_sub_ps(pFrame_00,pFrame_20),_mm_sub_ps(pFrame_02,pFrame_22)),pFrame_01),pFrame_01),pFrame_21),pFrame_21);

				__m128 sobel_result = _mm_sqrt_ps(_mm_add_ps(_mm_mul_ps(gx, gx), _mm_mul_ps(gy,gy)));
				
				if(!pad_threshold)
				{
					frame_sobel[index]=sobel_result.m128_f32[0];
					frame_sobel[index+1]=sobel_result.m128_f32[1];
					frame_sobel[index+2]=sobel_result.m128_f32[2];
					frame_sobel[index+3]=sobel_result.m128_f32[3];
					pSobelScreen[index]=(unsigned char)sobel_result.m128_f32[0];
					pSobelScreen[index+1]=(unsigned char)sobel_result.m128_f32[1];
					pSobelScreen[index+2]=(unsigned char)sobel_result.m128_f32[2];
					pSobelScreen[index+3]=(unsigned char)sobel_result.m128_f32[3];

					index+=4;
				}
				else
				{
					for(m=0;m<yuvinfo.w-yuvinfo.padding-i;m++)
					{
						frame_sobel[index+m]=sobel_result.m128_f32[m];
						pSobelScreen[index+m]=(unsigned char)sobel_result.m128_f32[m];
					}			
					index+=yuvinfo.w-yuvinfo.padding-i;
				}
			}
			pFrame_0 += yuvinfo.w;
			pFrame_1 += yuvinfo.w;
			pFrame_2 += yuvinfo.w;
			pad_threshold=0;
		}

		for(i=0;i<index;i+=4)
		{
			__m128 sobel_result_0 = _mm_set_ps (frame_sobel[i],frame_sobel[i+1], frame_sobel[i+2], frame_sobel[i+3]);
			sobel_avg_sum = _mm_add_ps(sobel_avg_sum,sobel_result_0);
		}		
		avg_frame_sobel=(sobel_avg_sum.m128_f32[0]+
			sobel_avg_sum.m128_f32[1]+
			sobel_avg_sum.m128_f32[2]+
			sobel_avg_sum.m128_f32[3])/index;
		avg_sobel = _mm_set_ps (avg_frame_sobel,avg_frame_sobel, avg_frame_sobel, avg_frame_sobel);

		for(i=0;i<index;i+=4)
		{
			__m128 sobel_result_1 = _mm_set_ps (frame_sobel[i],frame_sobel[i+1], frame_sobel[i+2], frame_sobel[i+3]);
			__m128 sobel_square=_mm_mul_ps(_mm_sub_ps(sobel_result_1,avg_sobel),_mm_sub_ps(sobel_result_1,avg_sobel));
			sobel_square_sum = _mm_add_ps(sobel_square_sum,sobel_square);
		}

		SI[k]=sqrt((sobel_square_sum.m128_f32[0]+sobel_square_sum.m128_f32[1]+sobel_square_sum.m128_f32[2]+sobel_square_sum.m128_f32[3])/index);
		avg_frame_sobel=0;
		sobel_avg_sum=_mm_set1_ps(+0.0f);
		sobel_square_sum=_mm_set1_ps(+0.0f);
		index=0;

			//���SI
			if(m_outputsi.GetCheck()!=FALSE){
			fprintf(fInfoSI, "%f\n", SI[k]);
			}
			si.push_back(SI[k]);

			CString curvalstr;
			curvalstr.Format(L"%.3f",SI[k]);
			dlg->m_curval.SetWindowText(curvalstr);

		if(m_outputsiyuv.GetCheck()!=FALSE){
			cfSI.Write(pSobelScreen, realframe_size);
			cfSI.Write(NewUVBuffer, realframe_size/2);
		}
		//�����������Ȳ����
		//cfSIData.Write(NewSIBuffer, nYSize*sizeof(float));
		if(dlg->m_combodraw.GetCurSel()==0){
			sdlparam.graphically=true;
		}else{
			sdlparam.graphically=false;
		}

		if(sdlparam.graphically==true){
			SDL_LockYUVOverlay(sdlparam.bmp);
			sdlparam.bmp->pixels[0]=pSobelScreen;
			sdlparam.bmp->pixels[2]=NewUVBuffer;
			sdlparam.bmp->pixels[1]=NewUVBuffer+realframe_size/4;     
			sdlparam.bmp->pitches[0]=yuvinfo.w-2*yuvinfo.padding;
			sdlparam.bmp->pitches[2]=(yuvinfo.w-2*yuvinfo.padding)/2;   
			sdlparam.bmp->pitches[1]=(yuvinfo.w-2*yuvinfo.padding)/2;
			SDL_UnlockYUVOverlay(sdlparam.bmp); 
			CRect screenrect;
			dlg->GetDlgItem(IDC_SCREEN)->GetWindowRect(screenrect);
			sdlparam.rect.x = 0;    
			sdlparam.rect.y = 0;    
			sdlparam.rect.w = screenrect.Width();    
			sdlparam.rect.h = screenrect.Height();    
			SDL_DisplayYUVOverlay(sdlparam.bmp, &sdlparam.rect); 
		}
		int progress=k*100/nFrameNum;
		dlg->m_progresscursi.SetPos(progress);
		CString progresstext;
		progresstext.Format(L"%d%%",progress);
		dlg->m_progresscursitext.SetWindowText(progresstext);
	}
	//FIX
	dlg->m_progresscursi.SetPos(100);
	dlg->m_progresscursitext.SetWindowText(L"100%");
	/*******************************����TI*********************************/
	if(m_outputtiyuv.GetCheck()!=FALSE){
	cfTI.Open(yuvinfo.url + _T("_TI.yuv"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite); 
	}
	//cfTIData.Open(strPath + _T("_TIData.y"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite); 

	for(k = 0;k < nFrameNum-1; k++)//���n֡��ѭ��n-1��
	{
		//Check State-----
		while(dlg->sysstate==SYSTEM_PAUSE){
			Sleep(1000);
		}
		if(dlg->sysstate==SYSTEM_PREPARE){
			return -1;
		}
		//----------------

		cfYUV.Seek(k*nFrameSize, CFile::begin);
		cfYUV.Read(pFrame, nYSize);

		cfYUV.Seek((k+1)*nFrameSize, CFile::begin);
		cfYUV.Read(pNextFrame, nYSize);

		pFrame_0=pFrame+yuvinfo.w*(yuvinfo.padding-1);
		pNextFrame_0=pNextFrame+yuvinfo.w*(yuvinfo.padding-1);

		for(j = yuvinfo.padding; j < yuvinfo.h-yuvinfo.padding; j++)
		{
			for(i = yuvinfo.padding; i < yuvinfo.w-yuvinfo.padding; i+=4)
			{
				if(i+4>yuvinfo.w-yuvinfo.padding)
					pad_threshold=1;
				// load 16 components. (0~6 will be used)
				__m128i current_0 = _mm_unpacklo_epi8(_mm_loadu_si128((__m128i*)(pFrame_0+i-1)), _mm_setzero_si128());
				__m128i next_0 = _mm_unpacklo_epi8(_mm_loadu_si128((__m128i*)(pNextFrame_0+i-1)), _mm_setzero_si128());
				// pFrame_00 = { pFrame_0[i-1], pFrame_0[i], pFrame_0[i+1], pFrame_0[i+2] }
				__m128 pFrame_00 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(current_0, _mm_setzero_si128()));
				__m128 pNextFrame_00 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(next_0, _mm_setzero_si128()));
				__m128 diff_result=_mm_sub_ps(pNextFrame_00,pFrame_00);
				if(!pad_threshold)
				{
					frame_diff[index]=diff_result.m128_f32[0];
					frame_diff[index+1]=diff_result.m128_f32[1];
					frame_diff[index+2]=diff_result.m128_f32[2];
					frame_diff[index+3]=diff_result.m128_f32[3];
					pDiffScreen[index]=(unsigned char)abs(diff_result.m128_f32[0]);
					pDiffScreen[index+1]=(unsigned char)abs(diff_result.m128_f32[1]);
					pDiffScreen[index+2]=(unsigned char)abs(diff_result.m128_f32[2]);
					pDiffScreen[index+3]=(unsigned char)abs(diff_result.m128_f32[3]);
					index+=4;
				}
				else
				{
					for(m=0;m<yuvinfo.w-yuvinfo.padding-i;m++)
					{
						frame_diff[index+m]=diff_result.m128_f32[m];
						pDiffScreen[index+m]=(unsigned char)abs(diff_result.m128_f32[m]);
					}
					index+=yuvinfo.w-yuvinfo.padding-i;
				}
			}
			pFrame_0 += yuvinfo.w;;
			pNextFrame_0 += yuvinfo.w;
			pad_threshold=0;
		}
		for(i=0;i<index;i+=4)
		{
			__m128 diff_result_0 = _mm_set_ps (frame_diff[i],frame_diff[i+1], frame_diff[i+2], frame_diff[i+3]);
			diff_avg_sum = _mm_add_ps(diff_avg_sum,diff_result_0);
		}
		avg_frame_diff=(diff_avg_sum.m128_f32[0]+
			diff_avg_sum.m128_f32[1]+
			diff_avg_sum.m128_f32[2]+
			diff_avg_sum.m128_f32[3])/index;
		avg_diff = _mm_set_ps (avg_frame_diff,avg_frame_diff, avg_frame_diff, avg_frame_diff);
		for(i=0;i<index;i+=4)
		{
			__m128 diff_result_1 = _mm_set_ps (frame_diff[i],frame_diff[i+1], frame_diff[i+2], frame_diff[i+3]);
			__m128 diff_square=_mm_mul_ps(_mm_sub_ps(diff_result_1,avg_diff),_mm_sub_ps(diff_result_1,avg_diff));
			diff_square_sum = _mm_add_ps(diff_square_sum,diff_square);
		}
		TI[k]=sqrt((diff_square_sum.m128_f32[0]+diff_square_sum.m128_f32[1]+diff_square_sum.m128_f32[2]+diff_square_sum.m128_f32[3])/index);
		avg_frame_diff=0;
		diff_avg_sum=_mm_set1_ps(+0.0f);
		diff_square_sum=_mm_set1_ps(+0.0f);

		index=0;
			
		if(m_outputtiyuv.GetCheck()!=FALSE){
			cfTI.Write(pDiffScreen, realframe_size);
			cfTI.Write(NewUVBuffer, realframe_size/2); 
		}

		//���TI
		if(m_outputti.GetCheck()!=FALSE){
			fprintf(fInfoTI, "%f\n", TI[k]);
		}
		ti.push_back(TI[k]);

		CString curvalstr;
		curvalstr.Format(L"%.3f",TI[k]);
		dlg->m_curval.SetWindowText(curvalstr);

		if(dlg->m_combodraw.GetCurSel()==0){
			sdlparam.graphically=true;
		}else{
			sdlparam.graphically=false;
		}


		if(sdlparam.graphically==true){
			SDL_LockYUVOverlay(sdlparam.bmp);
			sdlparam.bmp->pixels[0]=pDiffScreen;
			sdlparam.bmp->pixels[2]=NewUVBuffer;
			sdlparam.bmp->pixels[1]=NewUVBuffer+realframe_size/4;     
			sdlparam.bmp->pitches[0]=yuvinfo.w-2*yuvinfo.padding;
			sdlparam.bmp->pitches[2]=(yuvinfo.w-2*yuvinfo.padding)/2;   
			sdlparam.bmp->pitches[1]=(yuvinfo.w-2*yuvinfo.padding)/2;
			SDL_UnlockYUVOverlay(sdlparam.bmp); 

			CRect screenrect;
			dlg->GetDlgItem(IDC_SCREEN)->GetWindowRect(screenrect);
			sdlparam.rect.x = 0;    
			sdlparam.rect.y = 0;    
			sdlparam.rect.w = screenrect.Width();    
			sdlparam.rect.h = screenrect.Height();    
			SDL_DisplayYUVOverlay(sdlparam.bmp, &sdlparam.rect); 
		}

		int progress=k*100/(nFrameNum-1);
		dlg->m_progresscurti.SetPos(progress);
		CString progresstext;
		progresstext.Format(L"%d%%",progress);
		dlg->m_progresscurtitext.SetWindowText(progresstext);
	}
	//Draw
	 SDL_FreeYUVOverlay(sdlparam.bmp);
	//FIX
	dlg->m_progresscurti.SetPos(100);
	dlg->m_progresscurtitext.SetWindowText(L"100%");

	
	//�ļ���
	CString filename;
	int nPos=yuvinfo.url.ReverseFind('\\');   
	filename=yuvinfo.url.Mid(nPos+1);

	CString resulttistr(filename);
	CString resultsistr(filename);
	resulttistr.AppendFormat(L"\r\n=======\r\n");
	resultsistr.AppendFormat(L"\r\n=======\r\n");
	
	//����ƽ��ֵ
	for (int i=0;i<si.size();i++){
		resultsistr.AppendFormat(L"%.3f\r\n",si[i]);
	}

	for (int i=0;i<ti.size();i++){
		resulttistr.AppendFormat(L"%.3f\r\n",ti[i]);
	}

	resultsistr.AppendFormat(L"=======\r\n");
	resulttistr.AppendFormat(L"=======\r\n");

	dlg->resultdlg->AppendTIStr(resulttistr);
	dlg->resultdlg->AppendSIStr(resultsistr);



	//��¼TI��SI
	allsi.push_back(si);
	allti.push_back(ti);
	//���
	si.clear();
	ti.clear();
	//--------
	if(m_outputsiyuv.GetCheck()!=FALSE)
	cfSI.Close();

	if(m_outputtiyuv.GetCheck()!=FALSE)
	cfTI.Close();

	cfYUV.Close();
	//cfSIData.Close();
	//cfTIData.Close();
	if(m_outputsi.GetCheck()!=FALSE)
	fclose(fInfoSI);

	if(m_outputti.GetCheck()!=FALSE)
	fclose(fInfoTI);

	delete pFrame;
	delete pNextFrame;
	delete NewUVBuffer;
	delete pSobelScreen;
	delete pDiffScreen;
	delete frame_diff;
	delete frame_sobel;
	
	//���һ���ػ�����
	Invalidate();
	Sleep(1);
	return 0;
}

UINT Thread_Process(LPVOID lpParam){
	CTIandSIDlg *dlg=(CTIandSIDlg *)lpParam;

	dlg->m_progressall.SetPos(0);
	dlg->m_progressalltext.SetWindowText(L"0%");

	dlg->SetState(SYSTEM_PROCESS);
	//����
	int i,j;
	for(i=0;i<dlg->yuvlistvector.size();i++){
		dlg->m_yuvlist.SetCurSel(i);

		int progress=i/dlg->yuvlistvector.size();
		dlg->m_progressall.SetPos(progress);
		CString progresstext;
		progresstext.Format(L"%d%%",progress);
		dlg->m_progressalltext.SetWindowText(progresstext);
		//------------------------------------
		int retval=dlg->TIandSI(dlg->yuvlistvector[i],dlg);
		if(retval==-1){
			fcloseall();
			return -1;
		}
	}
	dlg->m_progressall.SetPos(100);
	dlg->m_progressalltext.SetWindowText(L"100%");

	dlg->SetState(SYSTEM_PREPARE);

	AfxMessageBox(L"������ϣ�");
	return 0;
}

void CTIandSIDlg::OnBnClickedOk()
{
	//���²���
	UpdateData(TRUE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SystemClear();
	if(yuvlistvector.size()<1){
		AfxMessageBox(L"�����б���������У�");
		return;
	}
	pThreadProcess=AfxBeginThread(Thread_Process,this);//�����߳�

}


void CTIandSIDlg::OnBnClickedYuvlistAdd()
{
	UpdateData(TRUE);
	//��ȡ��ַ�����б�
	CString yuvurl;
	YUVInfo yuvinfo={NULL,0,0,YUV420P};
	m_yuvurl.GetWindowText(yuvurl);

	if(yuvurl.IsEmpty()==TRUE){
		AfxMessageBox(L"�����ַΪ�գ�");
		return;
	}

	yuvinfo.url.Format(L"%s",yuvurl);
	yuvinfo.w=m_inputyuvwidth;
	yuvinfo.h=m_inputyuvheight;
	yuvinfo.padding=m_padding;

	yuvlistvector.push_back(yuvinfo);

	RefreshYuvlist();
}


void CTIandSIDlg::OnBnClickedYuvlistDelete()
{
	//ɾ��Ԫ��
	int urlindex=m_yuvlist.GetCurSel();
	if(urlindex!=-1){
	yuvlistvector.erase(yuvlistvector.begin()+urlindex);
	}else{
		AfxMessageBox(L"û��ѡ���κ�Ԫ�أ�");
	}

	RefreshYuvlist();
}


void CTIandSIDlg::OnBnClickedYuvlistDeleteall()
{
	if(yuvlistvector.size()==0){
		AfxMessageBox(L"�б��Ѿ�Ϊ�գ�");
	}
	//���Ԫ��
	yuvlistvector.clear();
	RefreshYuvlist();
}

void CTIandSIDlg::RefreshYuvlist(){
	//���������б�
	m_yuvlist.ResetContent();
	int i=0;
	for(i=0;i<yuvlistvector.size();i++){
		CString record;
		//�ļ���
		CString filename;
		int nPos=yuvlistvector[i].url.ReverseFind('\\');   
		filename=yuvlistvector[i].url.Mid(nPos+1);   
		record.Format(L"%s,%dx%d",filename,yuvlistvector[i].w,yuvlistvector[i].h);
		m_yuvlist.InsertString(i,record);
	}
}

void CTIandSIDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CTIandSIDlg::OnBnClickedPause()
{
	if(sysstate==SYSTEM_PROCESS){
		SetState(SYSTEM_PAUSE);
	}else{
		SetState(SYSTEM_PROCESS);
	}
}


void CTIandSIDlg::OnBnClickedStop()
{
	SetState(SYSTEM_PREPARE);
}


void CTIandSIDlg::OnBnClickedResult()
{
	resultdlg->ShowWindow(TRUE);
}


void CTIandSIDlg::SystemClear()
{
	resultdlg->SystemClear();
}

void CTIandSIDlg::SetState(Systemstate state)
{
	sysstate=state;
	switch(state){
	case SYSTEM_PREPARE:{
		m_outputti.EnableWindow(TRUE);
		m_outputtiyuv.EnableWindow(TRUE);
		m_outputsi.EnableWindow(TRUE);
		m_outputsiyuv.EnableWindow(TRUE);

		GetDlgItem(IDC_YUVLIST_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDC_YUVLIST_DELETEALL)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDPAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDSTOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_YUVLIST_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDPAUSE)->SetWindowText(L"��ͣ");
		break;
						}
	case SYSTEM_PAUSE:{
		GetDlgItem(IDPAUSE)->SetWindowText(L"����");
		break;
					  }
	case SYSTEM_PROCESS:{
		m_outputti.EnableWindow(FALSE);
		m_outputtiyuv.EnableWindow(FALSE);
		m_outputsi.EnableWindow(FALSE);
		m_outputsiyuv.EnableWindow(FALSE);

		GetDlgItem(IDC_YUVLIST_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_YUVLIST_DELETEALL)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDPAUSE)->EnableWindow(TRUE);
		GetDlgItem(IDSTOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_YUVLIST_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDPAUSE)->SetWindowText(L"��ͣ");
		state=SYSTEM_PROCESS;
		break;
						}
	}
}

void CTIandSIDlg::OnDropFiles(HDROP hDropInfo)
{
	LPTSTR pFilePathName =(LPTSTR)malloc(500);
	::DragQueryFile(hDropInfo, 0, pFilePathName,500);  // ��ȡ�Ϸ��ļ��������ļ�������ؼ���

	m_yuvurl.SetWindowText(pFilePathName);

	::DragFinish(hDropInfo);   // ע����������٣��������ͷ�Windows Ϊ�����ļ��ϷŶ�������ڴ�
	free(pFilePathName);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CTIandSIDlg::OnAdddlg()
{
		AddDlg adddlg;
		adddlg.maindlg=this;
		adddlg.DoModal();
		
}


void CTIandSIDlg::OnWebsite()
{
	ShellExecuteA(NULL, "open","http://blog.csdn.net/leixiaohua1020",NULL,NULL,SW_SHOWNORMAL);

}
