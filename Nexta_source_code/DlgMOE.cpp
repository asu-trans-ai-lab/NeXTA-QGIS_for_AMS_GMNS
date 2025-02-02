// DlgMOE.cpp : implementation file
//
//  Portions Copyright 2010 Xuesong Zhou (xzhou99@gmail.com)

//   If you help write or modify the code, please also list your names here.
//   The reason of having Copyright info here is to ensure all the modified version, as a whole, under the GPL 
//   and further prevent a violation of the GPL.

// More about "How to use GNU licenses for your own software"
// http://www.gnu.org/licenses/gpl-howto.html

//    This file is part of NeXTA Version 0.9.12022020 (Open-source).

//    NEXTA is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    NEXTA is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with NEXTA.  If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"
#include "Geometry.h"
#include "CSVParser.h"
#include "TLite.h"
#include "DlgMOE.h"
#include "Network.h"
#include <vector>
#include <algorithm>

extern CDlgMOE *g_LinkMOEDlg;
extern bool g_LinkMOEDlgShowFlag;
;


extern void g_SelectColorCode(CDC* pDC, int ColorCount);
extern void g_SelectColorCode2(CDC* pDC, int ColorCount);

extern void g_SelectThickColorCode(CDC* pDC, int ColorCount);

COLORREF s_ProjectColor=RGB(0,0,0);

CPen s_PenSimulationClock(PS_SOLID,2,RGB(0,255,0));

CPen s_PenHist(PS_SOLID,2,RGB(0,0,0));

CPen s_PenRealTimeProbe(PS_SOLID,2,RGB(0,0,255));
CBrush s_BrushRealTimeProbe(RGB(0,0,128));
CBrush s_BlackBrush(RGB(0,0,0));


CPen s_PenRealTimePrediction(PS_SOLID,3,RGB(0,255,255));

CPen s_PenRealTimeIrregularPrediction(PS_SOLID,1,RGB(255, 0, 255));


CPen s_PenTTI(PS_SOLID,0,RGB(0,0,255));
CBrush s_BrushTTI(RGB(0,255,0));

bool compare_AgentCFData (AgentCFData first, AgentCFData second)
{
	if(first.StartTime < second.StartTime) return true;
	else 
		return false;
}
// CDlgMOE dialog

IMPLEMENT_DYNAMIC(CDlgMOE, CDialog)

CDlgMOE::~CDlgMOE()
{
	g_LinkMOEDlg = NULL;
}

void CDlgMOE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_StartTime, m_StartHour);
	DDX_Control(pDX, IDC_COMBO_EndTime, m_EndHour);
	DDX_Control(pDX, IDC_COMBO_AggInterval, m_AggregationIntervalList);
 
}


BEGIN_MESSAGE_MAP(CDlgMOE, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_MOETYPE_LINKVOLUME, &CDlgMOE::OnMoetypeLinkvolume)
	ON_COMMAND(ID_MOETYPE_SPEED, &CDlgMOE::OnMoetypeSpeed)

	ON_UPDATE_COMMAND_UI(ID_MOETYPE_LINKVOLUME, &CDlgMOE::OnUpdateMoetypeLinkvolume)
	ON_UPDATE_COMMAND_UI(ID_MOETYPE_SPEED, &CDlgMOE::OnUpdateMoetypeSpeed)
	ON_COMMAND(ID_MOETYPE2_LINKVOLUME, &CDlgMOE::OnMoetype2Linkvolume)
	ON_COMMAND(ID_MOETYPE2_SPEED, &CDlgMOE::OnMoetype2Speed)
	ON_COMMAND(ID_MOETYPE2_NONE, &CDlgMOE::OnMoetype2None)
	ON_COMMAND(ID_DATA_EXPORT, &CDlgMOE::OnDataExport)
	ON_COMMAND(ID_MOETYPE1_LINKVOLUME, &CDlgMOE::OnMoetype1Linkvolume)
	ON_COMMAND(ID_MOETYPE1_SPEED, &CDlgMOE::OnMoetype1Speed)
	ON_COMMAND(ID_MOETYPE2_LINKVOLUME_VPH, &CDlgMOE::OnMoetype2LinkvolumeVph)
	ON_COMMAND(ID_MOETYPE2_SPEED_MPH, &CDlgMOE::OnMoetype2SpeedMph)

	ON_COMMAND(ID_MOETYPE2_DENSITY, &CDlgMOE::OnMoetype2Density)
	//ON_WM_LBUTTONDOWN()
	//ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONUP()
	//ON_WM_RBUTTONUP()
	//ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_VIEW_MOEVARIABILITYPLOT, &CDlgMOE::OnViewMoevariabilityplot)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOEVARIABILITYPLOT, &CDlgMOE::OnUpdateViewMoevariabilityplot)
	ON_COMMAND(ID_MOETYPE1_DENSITY, &CDlgMOE::OnMoetype1Density)
	ON_UPDATE_COMMAND_UI(ID_MOETYPE1_DENSITY, &CDlgMOE::OnUpdateMoetype1Density)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_DATA_EXPORT_WEATHER_DATA, &CDlgMOE::OnDataExportWeatherData)
	ON_COMMAND(ID_DATA_EXPORT_INCIDENT_DATA, &CDlgMOE::OnDataExportIncidentData)
	ON_COMMAND(ID_DATA_EXPORT_HIGH_DEMAND_DATA, &CDlgMOE::OnDataExportHighDemandData)
	ON_COMMAND(ID_DATA_EXPORT_SPECICAL_EVENT_DATA, &CDlgMOE::OnDataExportSpecicalEventData)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMULATIONVS, &CDlgMOE::OnUpdateViewSimulationvs)
	ON_COMMAND(ID_MOETYPE1_QUEUELENGTH, &CDlgMOE::OnMoetype1QueueLengthRatio)
	ON_COMMAND(ID_MOETYPE2_QUEUELENGTH, &CDlgMOE::OnMoetype2QueueLengthRatio)
	ON_COMMAND(ID_MOETYPE1_LINKOUTFLOWVOLUME, &CDlgMOE::OnMoetype1Linkoutflowvolume)
	ON_COMMAND(ID_DATA_FIXYRANGE, &CDlgMOE::OnDataFixyrange)
	ON_UPDATE_COMMAND_UI(ID_DATA_FIXYRANGE, &CDlgMOE::OnUpdateDataFixyrange)
	ON_CBN_SELCHANGE(IDC_COMBO_StartTime, &CDlgMOE::OnCbnSelchangeComboStarttime)
	ON_CBN_SELCHANGE(IDC_COMBO_EndTime, &CDlgMOE::OnCbnSelchangeComboEndtime)
	ON_CBN_SELCHANGE(IDC_COMBO_AggInterval, &CDlgMOE::OnCbnSelchangeComboAgginterval)
	ON_COMMAND(ID_MOETYPE2_LINKOUTFLOWVOLUME, &CDlgMOE::OnMoetype2Linkoutflowvolume)
	ON_COMMAND(ID_MOETYPE1_LINKTRAVELTIME, &CDlgMOE::OnMoetype1Linktraveltime)
	ON_COMMAND(ID_MOETYPE2_LINKTRAVELTIME, &CDlgMOE::OnMoetype2Linktraveltime)
	ON_CBN_SELCHANGE(IDC_COMBO_SimuDayNo, &CDlgMOE::OnCbnSelchangeComboSimudayno)
	ON_COMMAND(ID_DATA_SHOWSIMULATIONTIMESERIES, &CDlgMOE::OnDataShowsimulationtimeseries)
	ON_UPDATE_COMMAND_UI(ID_DATA_SHOWSIMULATIONTIMESERIES, &CDlgMOE::OnUpdateDataShowsimulationtimeseries)
	ON_COMMAND(ID_DATA_SHOWGRID, &CDlgMOE::OnDataShowgrid)
	ON_UPDATE_COMMAND_UI(ID_DATA_SHOWGRID, &CDlgMOE::OnUpdateDataShowgrid)
	ON_COMMAND(ID_MOE_NUMBEROFQUEUEDAgentS, &CDlgMOE::OnMoeNumberofqueuedAgents)
	ON_COMMAND(ID_MOE_NUMBEROFQUEUEDAgentS33788, &CDlgMOE::OnMoeNumberofqueuedAgents33788)
END_MESSAGE_MAP()


// CDlgMOE message handlers
void CDlgMOE::DrawTimeSeriesPlot()
{   
	CPaintDC dc(this); // device context for painting
	CRect PlotRect;
	GetClientRect(PlotRect);

	CRect PlotRectOrg = PlotRect;

	if(m_TimeLeft<0)
		m_TimeLeft = 0;
	if(m_TimeRight >=m_Range)
		m_TimeRight=m_Range;

	if(m_TimeRight< m_TimeLeft+m_MinDisplayInterval)
		m_TimeRight= m_TimeLeft+m_MinDisplayInterval;

	GetMaxLabelLength();

		PlotRect.right -= (155+m_max_project_string_length *4);

	if(Cur_MOE_type2== no_display)
	{
		PlotRect.top += 70;
		PlotRect.bottom -= 35;
		PlotRect.left += 60;

		DrawPlot(&dc,Cur_MOE_type1, PlotRect,true);


	}else
	{
		PlotRect.top += 70;
		PlotRect.bottom /=2;
		PlotRect.left += 60;

		DrawPlot(&dc, Cur_MOE_type1, PlotRect, true);

		PlotRect.top = PlotRectOrg.bottom /2+45;
		PlotRect.bottom = PlotRectOrg.bottom -20;
		PlotRect.left = PlotRectOrg.left+60;
		DrawPlot(&dc,Cur_MOE_type2, PlotRect,false);
	}
}
void CDlgMOE::DrawQKCurve()
{

	CPaintDC dc(this); // device context for painting
	CRect PlotRect;
	GetClientRect(PlotRect);


	CRect PlotRectOrg = PlotRect;

	PlotRect.bottom = PlotRect.bottom/3;  // reserve space for QK curve

	if(Cur_MOE_type2==no_display)
	{
		PlotRect.top += 35;
		PlotRect.bottom = PlotRectOrg.bottom/2-35;
		PlotRect.left += 60;
		PlotRect.right -= 100;

		DrawPlot(&dc,Cur_MOE_type1, PlotRect,true);
	}else
	{
		PlotRect.top += 20;
		PlotRect.bottom = PlotRectOrg.bottom/3-35;;
		PlotRect.left += 60;
		PlotRect.right -= 100;

		DrawPlot(&dc, Cur_MOE_type1, PlotRect, true);

		PlotRect.top = PlotRectOrg.bottom/3+45;
		PlotRect.bottom = PlotRectOrg.bottom*2/3 -20;
		PlotRect.left = PlotRectOrg.left+60;
		PlotRect.right = PlotRectOrg.right-100;

		DrawPlot(&dc,Cur_MOE_type2, PlotRect,false);

	}

	// now draw q-k curve
	PlotRect.top = PlotRectOrg.bottom*2/3+20;
	PlotRect.bottom = PlotRectOrg.bottom-20;

	dc.SelectObject(&s_BlackBrush);

	PlotRect.left = PlotRectOrg.left+60;
	PlotRect.right = PlotRectOrg.right/3;
	DrawSingleQKPlot(&dc,PlotRect);

	PlotRect.left = PlotRectOrg.right/3+40;
	PlotRect.right = PlotRectOrg.right*2/3-40;
	DrawSingleVKPlot(&dc,PlotRect);

	PlotRect.left = PlotRectOrg.right*2/3+40;
	PlotRect.right = PlotRectOrg.right-40;
	DrawSingleVQPlot(&dc,PlotRect);

}
void CDlgMOE::OnPaint()
{
	if(m_ViewMode == 0)
	{
		DrawTimeSeriesPlot();
	}


	if(m_ViewMode == 1)
	{
		DrawQKCurve();
	}
}


void CDlgMOE::DrawSingleQKPlot(CPaintDC* pDC, CRect PlotRect)
{

	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));

	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_DOT,0,RGB(0,0,0));

	pDC->TextOut(PlotRect.right/2,PlotRect.top-20,"Q-K Curve");

	pDC->SetBkMode(TRANSPARENT);

	m_YUpperBound = 2500;  // maximum lane flow

	float m_UnitDensity  = (float)(PlotRect.right - PlotRect.left)/(m_K_Jam);

	/// Y axis
	float YInterval = m_YUpperBound/10.0f;

	YInterval = (int)(YInterval)*1.0f;

	// data unit
	m_UnitData = 1;
	if((m_YUpperBound - m_YLowerBound)>0)
		m_UnitData = (float)(PlotRect.bottom - PlotRect.top)/(m_YUpperBound - m_YLowerBound);


	pDC->SelectObject(&TimePen);	


	char buff[20];
	for(int i=0;i<=m_K_Jam;i+=50)
	{
		if(i == 0)
			pDC->SelectObject(&NormalPen);
		else
			pDC->SelectObject(&DataPen);

		int XPosition=(long)(PlotRect.left+(i)*m_UnitDensity);
		pDC->MoveTo(XPosition,PlotRect.bottom+2);
		pDC->LineTo(XPosition,PlotRect.top);

		wsprintf(buff,"%d",i  );

		pDC->TextOut(XPosition,PlotRect.bottom+3,buff);
	}


	int i;
	for(i=m_YLowerBound; i <= m_YUpperBound + YInterval-1; i+= YInterval)
	{
		if( i > m_YUpperBound)
			i = m_YUpperBound;

		if(i == m_YLowerBound)
			pDC->SelectObject(&NormalPen);
		else
			pDC->SelectObject(&DataPen);

		int TimeYPosition= PlotRect.bottom - (int)((i*m_UnitData)+0.50);

		pDC->MoveTo(PlotRect.left-2, TimeYPosition);
		pDC->LineTo(PlotRect.right,TimeYPosition);

		wsprintf(buff,"%d",i);
		pDC->TextOut(PlotRect.left-55,TimeYPosition-5,buff);
	}

	int LinkCount = 0;

	std::list<s_link_selection>::iterator iLink;

	for (iLink = g_LinkDisplayList.begin(); iLink != g_LinkDisplayList.end(); iLink++,LinkCount++)
	{

		if(m_pDoc->m_DocumentNo == (*iLink).document_no )
		{
			DTALink* pLink = m_pDoc->m_LinkNoMap [(*iLink).link_no ];
			g_SelectColorCode(pDC,pLink->m_DisplayLinkID);

			for(i=m_TimeLeft;i<m_TimeRight;i+=1) // for each timestamp
			{

				if(pLink->IsSimulatedDataAvailable(i))
				{

					int x=(long)(PlotRect.left+(pLink->GetSimulatedDensity(i))*m_UnitDensity);

					int y= PlotRect.bottom - (int)((pLink->GetSimulatedLinkVolume(i)*m_UnitData));
					int size  =2;


					if(pLink->m_DisplayLinkID == 0)
						pDC->Ellipse(x-size,y-size,x+size,y+size);
					if(pLink->m_DisplayLinkID == 1)
						pDC->Rectangle(x-size,y-size,x+size,y+size);
					if(pLink->m_DisplayLinkID >= 2)
					{
						// cross
						pDC->MoveTo(x-size,y);
						pDC->LineTo(x+size,y);
						pDC->MoveTo(x,y-size);
						pDC->LineTo(x,y+size);
					}

				}

			}	
		}
	}

}

void CDlgMOE::DrawSingleVKPlot(CPaintDC* pDC, CRect PlotRect)
{

	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));

	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_DOT,0,RGB(0,0,0));

	pDC->TextOut(PlotRect.right/2+60,PlotRect.top-20,"V-K Curve");

	pDC->SetBkMode(TRANSPARENT);

	m_YUpperBound = 100;  // maximum speed

	float m_UnitDensity  = (float)(PlotRect.right - PlotRect.left)/(300);

	/// Y axis
	float YInterval = m_YUpperBound/10.0f;

	YInterval = (int)(YInterval)*1.0f;

	// data unit
	m_UnitData = 1;
	if((m_YUpperBound - m_YLowerBound)>0)
		m_UnitData = (float)(PlotRect.bottom - PlotRect.top)/(m_YUpperBound - m_YLowerBound);


	pDC->SelectObject(&TimePen);

	char buff[20];
	for(int i=0;i<=m_K_Jam;i+=50)
	{
		if(i == 0)
			pDC->SelectObject(&NormalPen);
		else
			pDC->SelectObject(&DataPen);

		int XPosition=(long)(PlotRect.left+(i)*m_UnitDensity);
		pDC->MoveTo(XPosition,PlotRect.bottom+2);
		pDC->LineTo(XPosition,PlotRect.top);

		wsprintf(buff,"%d",i  );

		pDC->TextOut(XPosition,PlotRect.bottom+3,buff);
	}


	int i;
	for(i=m_YLowerBound; i <= m_YUpperBound + YInterval-1; i+= YInterval)
	{
		if( i > m_YUpperBound)
			i = m_YUpperBound;

		if(i == m_YLowerBound)
			pDC->SelectObject(&NormalPen);
		else
			pDC->SelectObject(&DataPen);

		int TimeYPosition= PlotRect.bottom - (int)((i*m_UnitData)+0.50);

		pDC->MoveTo(PlotRect.left-2, TimeYPosition);
		pDC->LineTo(PlotRect.right,TimeYPosition);

		wsprintf(buff,"%d",i);
		pDC->TextOut(PlotRect.left-40,TimeYPosition-5,buff);
	}

	int LinkCount = 0;

	std::list<s_link_selection>::iterator iLink;

	for (iLink = g_LinkDisplayList.begin(); iLink != g_LinkDisplayList.end(); iLink++,LinkCount++)
	{
		if(m_pDoc->m_DocumentNo == (*iLink).document_no )
		{

			DTALink* pLink = m_pDoc->m_LinkNoMap [(*iLink).link_no ];

			g_SelectColorCode(pDC,pLink->m_DisplayLinkID);

			for(i=m_TimeLeft;i<  m_TimeRight;i+=1) // for each timestamp
			{
				if(pLink->IsSimulatedDataAvailable(i))
				{

					int x=(long)(PlotRect.left+(pLink->GetSimulatedDensity(i))*m_UnitDensity);

					int y= PlotRect.bottom - (int)((pLink->GetTDSpeed(i)*m_UnitData));
					int size  =2;


					if(pLink->m_DisplayLinkID == 0)
						pDC->Ellipse(x-size,y-size,x+size,y+size);
					if(pLink->m_DisplayLinkID == 1)
						pDC->Rectangle(x-size,y-size,x+size,y+size);
					if(pLink->m_DisplayLinkID >= 2)
					{
						// cross
						pDC->MoveTo(x-size,y);
						pDC->LineTo(x+size,y);
						pDC->MoveTo(x,y-size);
						pDC->LineTo(x,y+size);
					}
				}
			}
		}
	}

}

void CDlgMOE::DrawSingleVQPlot(CPaintDC* pDC, CRect PlotRect)
{

	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));

	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_DOT,0,RGB(0,0,0));

	pDC->TextOut(PlotRect.right/2+60,PlotRect.top-20,"V-Q Curve");

	pDC->SetBkMode(TRANSPARENT);

	m_YUpperBound = 100;  // maximum speed

	float m_UnitFlow  = (float)(PlotRect.right - PlotRect.left)/(m_Max_Flow);

	/// Y axis
	float YInterval = m_YUpperBound/10.0f;

	YInterval = (int)(YInterval)*1.0f;

	// data unit
	m_UnitData = 1;
	if((m_YUpperBound - m_YLowerBound)>0)
		m_UnitData = (float)(PlotRect.bottom - PlotRect.top)/(m_YUpperBound - m_YLowerBound);


	pDC->SelectObject(&TimePen);

	char buff[20];
	for(int i=0;i<=m_Max_Flow;i+=550)
	{
		if(i == 0)
			pDC->SelectObject(&NormalPen);
		else
			pDC->SelectObject(&DataPen);

		int XPosition=(long)(PlotRect.left+(i)*m_UnitFlow);
		pDC->MoveTo(XPosition,PlotRect.bottom+2);
		pDC->LineTo(XPosition,PlotRect.top);

		wsprintf(buff,"%d",i  );

		pDC->TextOut(XPosition,PlotRect.bottom+3,buff);
	}


	int i;
	for(i=m_YLowerBound; i <= m_YUpperBound + YInterval-1; i+= YInterval)
	{
		if( i > m_YUpperBound)
			i = m_YUpperBound;

		if(i == m_YLowerBound)
			pDC->SelectObject(&NormalPen);
		else
			pDC->SelectObject(&DataPen);

		int TimeYPosition= PlotRect.bottom - (int)((i*m_UnitData)+0.50);

		pDC->MoveTo(PlotRect.left-2, TimeYPosition);
		pDC->LineTo(PlotRect.right,TimeYPosition);

		wsprintf(buff,"%d",i);
		pDC->TextOut(PlotRect.left-40,TimeYPosition-5,buff);
	}

	int LinkCount = 0;

	std::list<s_link_selection>::iterator iLink;

	for (iLink = g_LinkDisplayList.begin(); iLink != g_LinkDisplayList.end(); iLink++,LinkCount++)
	{
		if(m_pDoc->m_DocumentNo == (*iLink).document_no )
		{

			DTALink* pLink = m_pDoc->m_LinkNoMap [(*iLink).link_no];
			g_SelectColorCode(pDC,pLink->m_DisplayLinkID);

			for(i=m_TimeLeft;i< m_TimeRight;i+=1) // for each timestamp
			{

				if(pLink->IsSimulatedDataAvailable(i))
				{
					int x=(long)(PlotRect.left+(pLink->m_LinkMOEAry[i].LinkFlow)*m_UnitFlow);

					int y= PlotRect.bottom - (int)((pLink->m_LinkMOEAry[i].Speed*m_UnitData));
					int size  =2;


					if(pLink->m_DisplayLinkID == 0)
						pDC->Ellipse(x-size,y-size,x+size,y+size);
					if(pLink->m_DisplayLinkID == 1)
						pDC->Rectangle(x-size,y-size,x+size,y+size);
					if(pLink->m_DisplayLinkID >= 2)
					{
						// cross
						pDC->MoveTo(x-size,y);
						pDC->LineTo(x+size,y);
						pDC->MoveTo(x,y-size);
						pDC->LineTo(x,y+size);
					}
				}
			}
		}
	}

}
void CDlgMOE::DrawPlot(CPaintDC* pDC,eLinkMOEMode  MOEType, CRect PlotRect, bool LinkTextFlag)
{
	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));
	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_DOT,0,RGB(0,0,0));

	pDC->SetBkMode(TRANSPARENT);

	double MaxY = GetMaxYValue(MOEType);


	if(MaxY < 0.0000001)  // no data
		MaxY  = 1;


	float y_interval = MaxY/5;


	float y_interval_resolution = g_FindClosestMOEYResolution(y_interval);

	if(m_bFixYRange == false)
		m_YUpperBound = y_interval_resolution*5;


	if(MOEType == link_queue_length_ratio)
	{
		m_YUpperBound = 100;
		y_interval_resolution = 20;
	}

	if (MOEType == number_of_queued_Agents)
	{
		m_YUpperBound = max(10, int(MaxY/10)*10);
		y_interval_resolution = 20;
	}
	if(MOEType == link_travel_time)
	{
		if(m_YUpperBound <1)
		{
			m_YUpperBound = 1;
			y_interval_resolution = 0.2;

		}
	}

	int TimeXPosition;

	int TimeInterval = g_FindClosestTimeResolution(m_TimeRight - m_TimeLeft);

	// time unit
	m_UnitTime = (float)(PlotRect.right - PlotRect.left )/max(1,(m_TimeRight - m_TimeLeft));

	/// Y axis
	float YInterval = y_interval_resolution;

	// data unit
	m_UnitData = (float)(PlotRect.bottom - PlotRect.top)/max(1,(m_YUpperBound - m_YLowerBound));

	pDC->SelectObject(&TimePen);

	CString buff;
	for(int i=m_TimeLeft;i<=m_TimeRight;i+=TimeInterval)
	{
		if(i == m_TimeLeft)
		{
			pDC->SelectObject(&NormalPen);

			i = int((m_TimeLeft/TimeInterval)+0.5)*TimeInterval; // reset time starting point
		}
		else
			pDC->SelectObject(&DataPen);

		TimeXPosition=(long)(PlotRect.left+(i-m_TimeLeft)*m_UnitTime);


		if(i>= m_TimeLeft)
		{
		if(m_bShowGrid || i ==m_TimeLeft || i ==m_TimeRight)
		{
			pDC->MoveTo(TimeXPosition,PlotRect.bottom+2);
			pDC->LineTo(TimeXPosition,PlotRect.top);
		}
			if(i/2 <10)
				TimeXPosition-=5;
			else
				TimeXPosition-=3;

			if(TimeInterval < 60)
			{
				int hour, min;
				hour = i/60;
				min =  i- hour*60;
				buff.Format("%2d:%02d",hour, min);
			}
			else
			{
				int min_in_a_day = i-int(i/1440*1440);

				if(min_in_a_day==0 && i> 1440)
					buff.Format("%dd", i/1440+1 );
				else
					buff.Format("%dh",min_in_a_day/60 );

			}

			pDC->TextOut(TimeXPosition,PlotRect.bottom+3,buff);
		}
	}


	pDC->SelectObject(&s_PenSimulationClock);

	if(g_Simulation_Time_Stamp >=m_TimeLeft && g_Simulation_Time_Stamp <= m_TimeRight )
	{
		TimeXPosition=(long)(PlotRect.left+(g_Simulation_Time_Stamp -m_TimeLeft)*m_UnitTime);

		pDC->MoveTo(TimeXPosition,PlotRect.bottom+2);
		pDC->LineTo(TimeXPosition,PlotRect.top);
	}
	float fi;



	//	YInterval = (int)(max(1,g_FindClosestYResolution(m_YUpperBound)));

	if( m_YUpperBound < m_YLowerBound +5* YInterval)
		m_YUpperBound = 5* YInterval;

	for(fi=m_YLowerBound; fi <= m_YUpperBound; fi+= YInterval)
	{
		if( fi > m_YUpperBound)
			fi = m_YUpperBound;

		if(fabs(fi-m_YLowerBound) < 0.000001)
			pDC->SelectObject(&NormalPen);
		else
			pDC->SelectObject(&DataPen);

		int TimeYPosition= PlotRect.bottom - (int)((fi*m_UnitData)+0.50);

		if(m_bShowGrid || fi == m_YLowerBound || fi ==m_YUpperBound)
		{
		pDC->MoveTo(PlotRect.left-2, TimeYPosition);
		pDC->LineTo(PlotRect.right,TimeYPosition);
		}
		if(YInterval>=1)
			buff.Format("%d",(int)(fi));
		else if (YInterval>=0.1)
			buff.Format("%.1f",fi);
		else if (YInterval>=0.01)
			buff.Format("%.2f",fi);
		else if (YInterval>=0.001)
			buff.Format("%.3f",fi);
		else if (YInterval>=0.0001)
			buff.Format("%.4f",fi);
		else if (YInterval>=0.00001)
			buff.Format("%.5f",fi);
		else
			buff.Format("%f",fi);

		pDC->TextOut(PlotRect.left-55,TimeYPosition-5,buff);
	}

	//Draw Time series
	if(MOEType == Agent_trajectory)
		DrawAgentTrajectory(MOEType, pDC, PlotRect,LinkTextFlag);
	else
	{
		m_LinkDisplayCount = 0;
		DrawTimeSeries(MOEType, pDC, PlotRect,LinkTextFlag);

	}


}

void CDlgMOE::OnSize(UINT nType, int cx, int cy)
{
	RedrawWindow();

	CDialog::OnSize(nType, cx, cy);
}

void CDlgMOE::GetMaxLabelLength()
{
	CString str_project;


	m_LinkDisplayCount = 0;
	std::list<s_link_selection>::iterator iLink;

	int DocNo = 0;
	std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	//find max project string length

	m_max_project_string_length = 0;

	while (iDoc != g_DocumentList.end())
	{
		if ((*iDoc)!=NULL && (*iDoc)->m_NodeSet.size()>0)
		{

			for (iLink = g_LinkDisplayList.begin(); iLink != g_LinkDisplayList.end(); iLink++)
			{

				if((*iDoc)->m_LinkNoMap.find((*iLink).link_no )  != (*iDoc)->m_LinkNoMap.end())
				{
				CString str_link;
				DTALink* pLink = (*iDoc)->m_LinkNoMap [(*iLink).link_no ];

				if(pLink!=NULL)
				{
				str_link.Format ("[%d] %d->%d,%s, ",
					m_LinkDisplayCount+1,pLink->m_FromNodeID, pLink->m_ToNodeID,(*iDoc)->m_ProjectTitle);

				int length_of_string = str_link.GetLength ();

				if(m_max_project_string_length < length_of_string)
					m_max_project_string_length = length_of_string; 
				
				}
				}

			}

		} //if there are nodes
		iDoc++;
	}

}
float CDlgMOE::GetMaxYValue(eLinkMOEMode MOEType)
{

	if(MOEType == link_queue_length_ratio)
		return 100;


	int start_time = 1440*30;
	int end_time = 0;

	m_DataVector.clear();

	float YMax = 0;
	std::list<s_link_selection>::iterator iLink;
	float value;
	std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	while (iDoc != g_DocumentList.end())
	{
		if ((*iDoc)->m_NodeSet.size()>0)
		{

			for (iLink = g_LinkDisplayList.begin(); iLink != g_LinkDisplayList.end(); iLink++)
			{
				if((*iDoc)->m_DocumentNo == (*iLink).document_no )
				{
					DTALink* pLink = (*iDoc)->m_LinkNoMap [(*iLink).link_no ];
					CTimeSeriesLine TSLine;

					if(pLink == NULL)
						continue;

					int TimeRight = m_TimeRight;



					CString str_MOE;
					switch (MOEType)
					{
					case lane_volume: str_MOE.Format ("Lane Outflow Volume: vphpl"); break;
					case speed: str_MOE.Format ("Speed: mp/h"); break;
					case link_outflow_volume: str_MOE.Format ("Link Outflow Volume: vph"); break;
					case link_travel_time: str_MOE.Format ("Link Travel Time: min"); break;
					case link_density: str_MOE.Format ("Density: vpmpl"); break;
					case link_queue_length_ratio: str_MOE.Format ("Queue Length as Perc of Link Length"); break;
					case number_of_queued_Agents: str_MOE.Format("Number of Queued Agents"); break;
					case link_traveltime:  str_MOE.Format("Travel Time Index: (Base = 100)"); break;
					}

					TSLine.time_series_label = str_MOE;

					for(int i= m_TimeLeft;i<TimeRight;i+=1) // for each timestamp
					{

						if(pLink->IsSimulatedDataAvailable(i))
						{

							value = pLink->GetWithinDayLinkMOE(MOEType,i);


							TSLine.value_map [i] = value;

							if(value > 0.00001 && pLink->GetSimulatedDensity(i)>=1)  // positive and valid value
							{
								if(i < start_time)
									start_time = i;

								if(i >end_time)
									end_time = i;
							}

							if(value > YMax)
								YMax = value;

						}
					}

					m_DataVector.push_back(TSLine);
				} // this document
			} // for all links
		}
		iDoc++;
	}  // for all documents



	if(m_bFirstLoadingFlag)
	{

		if(start_time >= 1440*30)
			start_time = 0; // no data

		if(end_time==0)
			end_time = 1440; // no data


		start_time = (int)(start_time/60)*60;
		end_time = (int)(end_time/60+1)*60;

		if( m_TimeLeft < start_time)
			m_TimeLeft = start_time;

		if( m_TimeRight > end_time)
			m_TimeRight = end_time;


		m_StartHour.SetCurSel(m_TimeLeft/60);
		m_EndHour.SetCurSel(m_TimeRight/60);

		m_bFirstLoadingFlag = false;
	}

	if(m_StartHour.GetCurSel()<0)
	{
		m_TimeLeft = 0;
		m_StartHour.SetCurSel(0);
	}
	if(m_EndHour.GetCurSel()<0)
	{
		m_TimeLeft = 1440;
		m_EndHour.SetCurSel(24);
	}



	if(m_TimeLeft >= m_TimeRight)  // no
	{
		m_TimeLeft = 0;
		m_TimeRight = min(1440, m_pDoc->m_SimulationStartTime_in_min + g_Simulation_Time_Horizon);
	}


	return YMax;
}

void CDlgMOE::DrawTimeSeries(eLinkMOEMode  MOEType , CPaintDC* pDC, CRect PlotRect,bool LinkTextFlag)
{
	int i;

	int Mod10 = 10;
	CString str_project;


	m_LinkDisplayCount = 0;
	std::list<s_link_selection>::iterator iLink;

	int DocNo = 0;
	std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	//find max project string length

	m_max_project_string_length = 0;


	iDoc = g_DocumentList.begin ();
	while (iDoc != g_DocumentList.end())
	{
		if ((*iDoc)->m_NodeSet.size()>0)
		{

			for (iLink = g_LinkDisplayList.begin(); iLink != g_LinkDisplayList.end(); iLink++)
			{
				if((*iDoc)->m_DocumentNo != (*iLink).document_no )
					continue;

				if((*iDoc)->m_LinkNoMap.find((*iLink).link_no ) != (*iDoc)->m_LinkNoMap.end()) 
				{	

					DTALink* pLink = (*iDoc)->m_LinkNoMap [(*iLink).link_no ];

					if (pLink == NULL)
						break;
					// draw time series
					bool b_ini_flag = false;

					if(DocNo==0) // main document
						g_SelectColorCode(pDC,m_LinkDisplayCount);
					else // alternative document
						g_SelectColorCode2(pDC,m_LinkDisplayCount+1);

					// draw legend
					// PlotRect.right
					long TimeYPosition;


					CString str_MOE;
					switch (MOEType)
					{
					case lane_volume: str_MOE.Format ("Lane Outflow Volume: vphpl"); break;
					case speed: str_MOE.Format ("Speed: km/h"); break;
					case link_outflow_volume: str_MOE.Format ("Link Outflow Volume: vph"); break;
					case link_travel_time: str_MOE.Format ("Link Travel Time: min"); break;
					case link_queue_length_ratio: str_MOE.Format ("Queue Length as Perc of Link Length"); break;
					case number_of_queued_Agents: str_MOE.Format("Number of Queued Agents"); break;
					case link_density: str_MOE.Format("Density: vpmpl"); break;
					case link_traveltime:  str_MOE.Format ("Travel Time Index: (Base = 100)"); break;
					}
					pDC->TextOut(PlotRect.right/2,PlotRect.top-20,str_MOE);

					if(LinkTextFlag)
					{
						TimeYPosition= PlotRect.top + (int)(m_LinkDisplayCount*30);
						if(m_bShowSimulationData)
						{
						pDC->MoveTo(PlotRect.right+5,TimeYPosition);
						pDC->LineTo(PlotRect.right+30,TimeYPosition);

						CString str_link;

						str_link.Format ("[%d] %d->%d,%s, ",
							m_LinkDisplayCount+1,pLink->m_FromNodeID, pLink->m_ToNodeID,(*iDoc)->m_ProjectTitle);

						int length_of_string = str_link.GetLength ();

						pDC->TextOut(PlotRect.right+5-(m_max_project_string_length*4),TimeYPosition,str_link);
						}
					}

					long TimeXPosition=PlotRect.left;

					// draw normal MOE result

					if(m_bShowSimulationData)
					{
					b_ini_flag = false;

					for(i=m_TimeLeft;i<m_TimeRight;i+=1) // for each timestamp
					{

						if (pLink->IsSimulatedDataAvailable(i))
						{

							TimeXPosition=(long)(PlotRect.left+(i-m_TimeLeft)*m_UnitTime);

							float value = pLink->GetWithinDayLinkMOE(MOEType,i);

							// show both probe data and sensor data
							TimeYPosition= PlotRect.bottom - (int)((value*m_UnitData));

							if(!b_ini_flag)
							{
								pDC->MoveTo(TimeXPosition, TimeYPosition);
								b_ini_flag = true;
							}

							pDC->LineTo(TimeXPosition, TimeYPosition);
							if(value>1 && i%5==0)
							{
								TRACE("data : %s, %d,%f\n",(*iDoc)->m_ProjectTitle,i,value);
							}

						}
					}
					}
							
			


					m_LinkDisplayCount++;
				} // found link
			}  // for link

			DocNo++;
		} //if there are nodes
		iDoc++;
	}

}

BOOL CDlgMOE::OnInitDialog()
{
	m_bFirstLoadingFlag = true;
	m_TimeLeft  = 0;
	m_TimeRight = m_pDoc->m_SimulationStartTime_in_min  + g_Simulation_Time_Horizon ;


	CDialog::OnInitDialog();

	m_AggregationValueVector.push_back(1);
	m_AggregationValueVector.push_back(5);
	m_AggregationValueVector.push_back(15);
	m_AggregationValueVector.push_back(30);
	m_AggregationValueVector.push_back(60);
	m_AggregationValueVector.push_back(120);
	m_AggregationValueVector.push_back(1440);

	for(unsigned int i = 0;  i< m_AggregationValueVector.size (); i++)
	{
		CString str;
		str.Format("%d min",m_AggregationValueVector[i]);

		m_AggregationIntervalList.AddString (str);

		if(g_MOEAggregationIntervalInMin  == m_AggregationValueVector[i])
		{
			m_AggregationIntervalList.SetCurSel (i);

		}

	}

	for (int i = 0; i<= 24; i++)
	{
		CString str;
		str.Format("%d",i);

		m_StartHour.AddString (str);
		m_EndHour.AddString (str);
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMOE::OnMoetypeLinkvolume()
{
	Cur_MOE_type1 = lane_volume;
	Invalidate();
}

void CDlgMOE::OnMoetypeSpeed()
{
	Cur_MOE_type1 = speed;
	SetWindowText("Speed mph");
	Invalidate();}



void CDlgMOE::OnUpdateMoetypeLinkvolume(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Cur_MOE_type1 == lane_volume);

}

void CDlgMOE::OnUpdateMoetypeSpeed(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Cur_MOE_type1 == speed);
}



void CDlgMOE::OnMoetype2Linkvolume()
{
	Cur_MOE_type2 = lane_volume;
	Invalidate();
}

void CDlgMOE::OnMoetype2Speed()
{
	Cur_MOE_type2 = speed;
	Invalidate();
}


void CDlgMOE::OnMoetype2None()
{
	Cur_MOE_type2 = no_display;
	Invalidate();
}

void CDlgMOE::OnDataExport()
{
	ExportData(0);
}

void CDlgMOE::ExportData(int EventType)
{
	CString Link_MOE_File_Name = m_pDoc->m_ProjectDirectory +"export_linkMOE_.csv";
	{
		CWaitCursor wait;

		if(ExportDataToCSVFile(Link_MOE_File_Name))
		{
			m_pDoc->OpenCSVFileInExcel (Link_MOE_File_Name);
		}
	}
}

bool CDlgMOE::ExportDataToCSVFile(CString csv_file)
{

	CCSVWriter DataFile;

	// Convert a TCHAR string to a LPCSTR

	if(DataFile.Open( m_pDoc->CString2StdString(csv_file)))
	{

		DataFile.SetFieldName ("project_name");
		DataFile.SetFieldName ("aggregation_interval (min)");
		DataFile.SetFieldName ("display_link_index");
		DataFile.SetFieldName ("from_node_id");
		DataFile.SetFieldName ("to_node_id");
		DataFile.SetFieldName ("link_name");
		DataFile.SetFieldName ("timestamp (min)");
		DataFile.SetFieldName ("hour");
		DataFile.SetFieldName ("min");
		DataFile.SetFieldName ("Link Inflow Volume (vph)");
		DataFile.SetFieldName ("Link Outflow Volume (vph)");
		DataFile.SetFieldName ("Lane Inflow Volume(vphpl)");
		DataFile.SetFieldName ("Speed");
		DataFile.SetFieldName ("Density (vpmpl)");
		DataFile.SetFieldName ("Queue Length as Percentage of Link Length");
		DataFile.SetFieldName ("Cumulative Arrival Count");
		DataFile.SetFieldName ("Cumulative Departure Count");

	
		DataFile.WriteHeader ();


		m_LinkDisplayCount = 1;
		std::list<s_link_selection>::iterator iLink;

		int DocNo = 0;
		std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
		while (iDoc != g_DocumentList.end())
		{
			if ((*iDoc)->m_NodeSet.size()>0)
			{

				for (iLink = g_LinkDisplayList.begin(); iLink != g_LinkDisplayList.end(); iLink++)
				{
					if((*iDoc)->m_DocumentNo != (*iLink).document_no )
						continue;

					if((*iDoc)->m_LinkNoMap.find((*iLink).link_no ) != (*iDoc)->m_LinkNoMap.end()) 
					{	

						DTALink* pLink = (*iDoc)->m_LinkNoMap [(*iLink).link_no ];
						// draw time series
						bool b_ini_flag = false;


						//for(int i=m_TimeLeft;i<m_TimeRight;i+=1) // for each timestamp
						for (int i = m_TimeLeft; i <= m_TimeLeft; i += 1) // for each timestamp
							{

							if (pLink->IsSimulatedDataAvailable(i))


							{

								string project_name = (*iDoc)->CString2StdString ((*iDoc)->m_ProjectTitle);
								DataFile.SetValueByFieldName ("project_name",project_name);
								DataFile.SetValueByFieldName ("aggregation_interval (min)",g_MOEAggregationIntervalInMin);
								DataFile.SetValueByFieldName ("link_id", pLink->m_LinkID);

								DataFile.SetValueByFieldName ("from_node_id", pLink->m_FromNodeID );
								DataFile.SetValueByFieldName ("to_node_id", pLink->m_ToNodeID );
								DataFile.SetValueByFieldName ("link_name", pLink->m_Name);
								DataFile.SetValueByFieldName ("timestamp (min)",i);
								int hour = (int)(i/60);
								DataFile.SetValueByFieldName ("hour",hour);
								int min = i - hour;
								DataFile.SetValueByFieldName ("min",min);

								float value = pLink->GetSimulatedLinkVolume (i);
								DataFile.SetValueByFieldName ("Link Inflow Volume (vph)", value);
								value = pLink->GetSimulatedLinkOutVolume (i);
								DataFile.SetValueByFieldName ("Link Outflow Volume (vph)",value);
								value = pLink->GetSimulatedLaneVolume (i);
								DataFile.SetValueByFieldName ("Lane Inflow Volume(vphpl)",value);
								value = pLink->GetTDSpeed(i);
								DataFile.SetValueByFieldName ("Speed", value);

								
								value = pLink->GetSimulatedDensity(i);
								DataFile.SetValueByFieldName ("Density (vpmpl)", value);
								value = pLink->GetQueueLengthRatioPercentage(i);
								DataFile.SetValueByFieldName ("Queue Length as Percentage of Link Length", value);


								value =  pLink->GetArrivalCumulativeFlow(i);
								DataFile.SetValueByFieldName ("Cumulative Arrival Count", value);
								value = pLink->GetDepartureCumulativeFlow(i);
								DataFile.SetValueByFieldName ("Cumulative Departure Count", value);


								DataFile.WriteRecord ();
							}



							m_LinkDisplayCount++;
						}  //for time
					} // found link
				}  // for link

				DocNo++;
			} //if there are nodes
			iDoc++;
		}



		return true;
	}
	else
		return false;
}

void CDlgMOE::OnMoetype1Linkvolume()
{
	Cur_MOE_type1 = link_outflow_volume;
	Invalidate();
}

void CDlgMOE::OnMoetype1Speed()
{
	Cur_MOE_type1 = speed;
	Invalidate();
}

void CDlgMOE::OnMoetype2LinkvolumeVph()
{
	Cur_MOE_type2 = link_outflow_volume;
	Invalidate();
}

void CDlgMOE::OnMoetype2SpeedMph()
{
	Cur_MOE_type2 = speed;
	Invalidate();
}

void CDlgMOE::OnViewMoetimeseries()
{
	m_ViewMode = no_display;
	Invalidate();
}

void CDlgMOE::OnViewQ()
{
	m_ViewMode = lane_volume;
	Invalidate();
}

void CDlgMOE::OnMoetype3Q()
{
	m_ViewMode = 1;
	Invalidate();
}

void CDlgMOE::OnMoetype2Density()
{
	Cur_MOE_type2 = link_density;
	Invalidate();
}

void CDlgMOE::OnLButtonDown(UINT nFlags, CPoint point)
{
	//m_last_cpoint = point;
	//AfxGetApp()->LoadCursor(IDC_MOVENETWORK);
	//m_bMoveDisplay = true;

	CDialog::OnLButtonDown(nFlags, point);
	Invalidate();
}

void CDlgMOE::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);

}

void CDlgMOE::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bMoveDisplay)
	{
		CSize OffSet = point - m_last_cpoint;
		int time_shift = max(1,OffSet.cx/m_UnitTime);
		m_TimeLeft-= time_shift;
		m_TimeRight-= time_shift;

		AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		m_bMoveDisplay = false;
		Invalidate();
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgMOE::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_TimeLeft = 0 ;
	m_TimeRight = m_Range ;
	Invalidate();

	CDialog::OnRButtonUp(nFlags, point);
}

BOOL CDlgMOE::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CDlgMOE::OnViewResettimerange()
{
	m_TimeLeft = 0 ;
	m_TimeRight = m_Range ;
	Invalidate();
}

void CDlgMOE::OnEstimationHistoricalavgpattern()
{
	m_bShowHistPattern = !m_bShowHistPattern;
	Invalidate();
}

void CDlgMOE::OnUpdateEstimationHistoricalavgpattern(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowHistPattern);
}

void CDlgMOE::OnViewMoevariabilityplot()
{
	m_bShowVariability = !m_bShowVariability;
	if(m_bShowVariability)
	{
		m_TimeLeft = 0;
		m_TimeRight = 1440;
	}else
	{
		m_TimeLeft = 0;
		m_TimeRight = m_Range;
	}
	Invalidate();

}

void CDlgMOE::OnUpdateViewMoevariabilityplot(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowVariability);
}

void CDlgMOE::OnMoetype1Traveltime()
{
	Cur_MOE_type1 = link_traveltime;
	Invalidate();
}

void CDlgMOE::OnEstimationPrediction()
{
	m_bShowPrediction = !m_bShowPrediction;
	Invalidate();
}

void CDlgMOE::OnMoetype3None()
{
	m_ViewMode = 0;
	Invalidate();
}

void CDlgMOE::OnMoetype1Density()
{
	Cur_MOE_type1 = link_density;
	Invalidate();
}

void CDlgMOE::OnUpdateMoetype1Density(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CDlgMOE::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}
void CDlgMOE::OnEstimationShoweventlabel()
{
	m_bShowEventLabel = !m_bShowEventLabel;
	Invalidate();

}

void CDlgMOE::OnUpdateEstimationShoweventlabel(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowEventLabel);
}

void CDlgMOE::OnDataExportWeatherData()
{
	ExportData(1);
}

void CDlgMOE::OnDataExportIncidentData()
{
	ExportData(3);
}

void CDlgMOE::OnDataExportHighDemandData()
{
	ExportData(2);
}

void CDlgMOE::OnDataExportSpecicalEventData()
{
	ExportData(4);
}

void CDlgMOE::OnEstimationShowweatherdata()
{
	m_bShowWeatherLabel = !m_bShowWeatherLabel;
	Invalidate();

}

void CDlgMOE::OnUpdateEstimationShowweatherdata(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(m_bShowWeatherLabel);

}



void CDlgMOE::CarFollowingSimulation()
{
}
void CDlgMOE::OnMoetype1Agenttrajectory()
{
	Cur_MOE_type1 = Agent_trajectory;
	m_DisplayAgentResolution = 1;
	Invalidate();
}


void CDlgMOE::DrawAgentTrajectory(eLinkMOEMode  MOEType , CPaintDC* pDC, CRect PlotRect,bool LinkTextFlag)
{
	// draw normal MOE result
	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));
	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_SOLID,0,RGB(0,0,0));

	pDC->SetBkMode(TRANSPARENT);

	int Mod10 = 10;
	CString str_project;

	int LinkCount = 0;

	std::list<s_link_selection>::iterator iLink;

	for (iLink = g_LinkDisplayList.begin(); iLink != g_LinkDisplayList.end(); iLink++,LinkCount++)
	{
		DTALink* pLink = m_pDoc->m_LinkNoMap [(*iLink).link_no ];
		// draw legend
		// PlotRect.right
		long TimeYPosition;

		CString str_MOE;
		switch (MOEType)
		{
		case Agent_trajectory: str_MOE.Format ("Agent Trajectory"); break;
		}
		pDC->TextOut(PlotRect.right/2,PlotRect.top-20,str_MOE);

		if(LinkTextFlag)
		{
			TimeYPosition= PlotRect.top + (int)(LinkCount*20);
			pDC->MoveTo(PlotRect.right+5,TimeYPosition);
			pDC->LineTo(PlotRect.right+30,TimeYPosition);

			CString str_link;
			str_link.Format ("[%d] %d->%d",pLink->m_DisplayLinkID,pLink->m_FromNodeID, pLink->m_ToNodeID);
			pDC->TextOut(PlotRect.right,TimeYPosition,str_link);
			//			str_link.Format ("%d Veh",m_NumberOfAgents);
			//			pDC->TextOut(PlotRect.right,TimeYPosition+25,str_link);
		}

		long TimeXPosition=PlotRect.left;

		CWaitCursor wc;	
		CarFollowingSimulation();  // perform car following simulation

		m_UnitTime = 1;
		if((m_TimeRight - m_TimeLeft)>0)
			m_UnitTime = (float)(PlotRect.right - PlotRect.left)/(m_TimeRight - m_TimeLeft);

		float m_UnitDistance = (float)(PlotRect.bottom  - PlotRect.top)/(m_YUpperBound - 	m_YLowerBound);


		pDC->SelectObject(&TimePen);

		int TimeInterval = (m_TimeRight - m_TimeLeft)/m_MinDisplayInterval;

		for(int i=m_TimeLeft;i<=m_TimeRight;i+=TimeInterval)
		{
			if(i == m_TimeLeft)
			{
				pDC->SelectObject(&NormalPen);

				i = int((m_TimeLeft/TimeInterval)+0.5)*TimeInterval; // reset time starting point
			}
			else
				pDC->SelectObject(&DataPen);

			TimeXPosition=(long)(PlotRect.left+(i-m_TimeLeft)*m_UnitTime);

			if(i>= m_TimeLeft)
			{
				if(i/2 <10)
					TimeXPosition-=5;
				else
					TimeXPosition-=3;

			}
		}

		pDC->SelectObject(&s_PenSimulationClock);
		if(g_Simulation_Time_Stamp >=m_TimeLeft && g_Simulation_Time_Stamp <= m_TimeRight )
		{
			TimeXPosition=(long)(PlotRect.left+(g_Simulation_Time_Stamp -m_TimeLeft)*m_UnitTime);
			pDC->MoveTo(TimeXPosition,PlotRect.bottom+2);
			pDC->LineTo(TimeXPosition,PlotRect.top);
		}

		//	if(m_bDataAvailable)
		{
			pDC->SelectObject(&DataPen);
			int t,v;
			int visul_time_resolution_per_simulation  = 1;
			for(v=0; v<m_NumberOfAgents; v+=m_DisplayAgentResolution)
			{
				float StartTime = m_AgentDataList[v].StartTime;

				if(StartTime < m_TimeLeft)
					StartTime = m_TimeLeft;

				float EndTime = m_AgentDataList[v].EndTime;

				if(EndTime > m_TimeRight)
					EndTime = m_TimeRight;

				bool bIniFlag  = false;
				if(StartTime< EndTime)
				{
					for(t= StartTime*m_CarFollowingTimeResolutionPerMin; t<=EndTime*m_CarFollowingTimeResolutionPerMin; t+=visul_time_resolution_per_simulation)  
					{

						int x=(int)(PlotRect.left+(t*1.0f/m_CarFollowingTimeResolutionPerMin - m_TimeLeft)*m_UnitTime);
						int y = PlotRect.bottom - (int)((m_AgentDistanceAry[v][t]*m_UnitDistance)+0.50);

						//			if(v<=10)
						//				TRACE("Veh: %d, %d, %f, y = %d\n",v,t,m_AgentDistanceAry[v][t],y);

						if(!bIniFlag)
						{
							pDC->MoveTo(x,y);
							bIniFlag = true;

						}else
						{
							pDC->LineTo(x,y);

						}

					}

				}
			}
		}

		break;  // only for the first link
	}


}

void CDlgMOE::OnMoetype1Agenttrajectory10()
{
	Cur_MOE_type1 = Agent_trajectory;
	m_DisplayAgentResolution = 10;
	Invalidate();
}

void CDlgMOE::OnMoetype1Agenttrajectory30()
{
	Cur_MOE_type1 = Agent_trajectory;
	m_DisplayAgentResolution = 30;
	Invalidate();
}



void CDlgMOE::OnDataExportAgenttrajectory()
{

}

void CDlgMOE::OnUpdateEstimationPrediction(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowPrediction);

}



void CDlgMOE::OnUpdateViewSimulationvs(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSensorData);
}

void CDlgMOE::OnMoetype1QueueLengthRatio()
{
	Cur_MOE_type1 = link_queue_length_ratio;
	Invalidate();
}

void CDlgMOE::OnMoetype2QueueLengthRatio()
{
	Cur_MOE_type2 = link_queue_length_ratio;
	Invalidate();
}

void CDlgMOE::OnMoetype1Linkoutflowvolume()
{
	Cur_MOE_type1 = link_outflow_volume;
	Invalidate();

}

void CDlgMOE::OnDataFixyrange()
{
	m_bFixYRange = !m_bFixYRange;
}

void CDlgMOE::OnUpdateDataFixyrange(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bFixYRange);
}

void CDlgMOE::OnCbnSelchangeComboStarttime()
{
	int sel = m_StartHour.GetCurSel();

	if((sel)* 60 < m_TimeRight)
	{
		m_TimeLeft = (sel)* 60;
	}else
	{// restore
		m_StartHour.SetCurSel (m_TimeLeft/60);

	}
	Invalidate();
}

void CDlgMOE::OnCbnSelchangeComboEndtime()
{
	int sel = m_EndHour.GetCurSel();

	if(m_TimeLeft <(sel)* 60)
	{
		m_TimeRight = (sel)* 60;
	}else
	{  // restore
		m_EndHour.SetCurSel(m_TimeRight/60);

	}

	Invalidate();
}

void CDlgMOE::OnCbnSelchangeComboAgginterval()
{
	g_MOEAggregationIntervalInMin = m_AggregationValueVector [ m_AggregationIntervalList.GetCurSel()];

	//
	Invalidate ();

}

void CDlgMOE::OnMoetype2Linkoutflowvolume()
{
	Cur_MOE_type2 = link_outflow_volume;
	Invalidate();
}

void CDlgMOE::OnMoetype1Linktraveltime()
{
	Cur_MOE_type1 = 	link_travel_time ;
	Invalidate();
}

void CDlgMOE::OnMoetype2Linktraveltime()
{
	Cur_MOE_type2 = 	link_travel_time ;
	Invalidate();
}

void CDlgMOE::OnCbnSelchangeComboSimudayno()
{

	Invalidate();

}
void CDlgMOE::OnDataShowsimulationtimeseries()
{
	m_bShowSimulationData = !m_bShowSimulationData;
	Invalidate();
}

void CDlgMOE::OnUpdateDataShowsimulationtimeseries(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSimulationData);
}
void CDlgMOE::OnDataShowgrid()
{
	m_bShowGrid = !m_bShowGrid;
	Invalidate();
}

void CDlgMOE::OnUpdateDataShowgrid(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowGrid);
}


void CDlgMOE::OnMoeNumberofqueuedAgents()
{
	Cur_MOE_type1 = number_of_queued_Agents;
	Invalidate();

}


void CDlgMOE::OnMoeNumberofqueuedAgents33788()
{
	Cur_MOE_type2 = number_of_queued_Agents;
	Invalidate();
}
