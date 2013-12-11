/*****************************************************************************

   ģ����      : KDV define 

   �ļ���      : kdvdef.h

   ����ļ�    : 

   �ļ�ʵ�ֹ���: KDV�궨��

   ����        : κ�α�

   �汾        : V3.0  Copyright(C) 2001-2002 KDC, All rights reserved.

-----------------------------------------------------------------------------

   �޸ļ�¼:

   ��  ��      �汾        �޸���      �޸�����

   2004/02/17  3.0         κ�α�        ����

******************************************************************************/

#ifndef _KDV_DEFINE_H_

#define _KDV_DEFINE_H_



#ifdef __cplusplus

extern "C" {

#endif /* __cplusplus */





/*ý�����Ͷ���*/

#define   MEDIA_TYPE_NULL                 (u8)255  /*ý������Ϊ��*/
	
/*��Ƶ*/

#define	  MEDIA_TYPE_MP3	              (u8)96  /*mp3 mode 0-4*/

#define   MEDIA_TYPE_G7221C	              (u8)98  /*G722.1.C Siren14*/

#define   MEDIA_TYPE_G719	              (u8)99  /*G719 non-standard of polycom serial 22*/

#define   MEDIA_TYPE_PCMA		          (u8)8   /*G.711 Alaw  mode 5*/

#define   MEDIA_TYPE_PCMU		          (u8)0   /*G.711 ulaw  mode 6*/

#define   MEDIA_TYPE_G721		          (u8)2   /*G.721*/

#define   MEDIA_TYPE_G722		          (u8)9   /*G.722*/

#define	  MEDIA_TYPE_G7231		          (u8)4   /*G.7231*/

#define   MEDIA_TYPE_ADPCM                (u8)5   /*DVI4 ADPCM*/

#define	  MEDIA_TYPE_G728		          (u8)15  /*G.728*/

#define	  MEDIA_TYPE_G729		          (u8)18  /*G.729*/
#define   MEDIA_TYPE_G7221                (u8)13  /*G.7221*/

#define   MEDIA_TYPE_AACLC                (u8)102 /*AAC LC*/
#define   MEDIA_TYPE_AACLD                (u8)103 /*AAC LD*/

/*��Ƶ*/

#define   MEDIA_TYPE_MP4	              (u8)97  /*MPEG-4*/

#define   MEDIA_TYPE_H261	              (u8)31  /*H.261*/

#define   MEDIA_TYPE_H262	              (u8)33  /*H.262 (MPEG-2)*/

#define   MEDIA_TYPE_H263	              (u8)34  /*H.263*/

#define   MEDIA_TYPE_H263PLUS             (u8)101 /*H.263+*/

#define   MEDIA_TYPE_H264	              (u8)106 /*H.264*/

#define   MEDIA_TYPE_H264_ForHuawei       (u8)105 /*H.264*/

#define	  MEDIA_TYPE_FEC					(u8)107 /* fec custom define */

/*����*/
#define   MEDIA_TYPE_H224	              (u8)100  /*H.224 Payload �ݶ�100*/
#define   MEDIA_TYPE_T120                 (u8)200  /*T.120ý������*/
#define   MEDIA_TYPE_H239                 (u8)239  /*H.239�������� */
#define   MEDIA_TYPE_MMCU                 (u8)120  /*��������ͨ�� */
	

/*kdvԼ���ı��ط���ʱʹ�õĻý������ */
#define   ACTIVE_TYPE_PCMA		          (u8)110   /*G.711 Alaw  mode 5*/
#define   ACTIVE_TYPE_PCMU		          (u8)111   /*G.711 ulaw  mode 6*/
#define   ACTIVE_TYPE_G721		          (u8)112   /*G.721*/
#define   ACTIVE_TYPE_G722		          (u8)113   /*G.722*/
#define	  ACTIVE_TYPE_G7231		          (u8)114   /*G.7231*/
#define	  ACTIVE_TYPE_G728		          (u8)115   /*G.728*/
#define	  ACTIVE_TYPE_G729		          (u8)116   /*G.729*/
#define   ACTIVE_TYPE_G7221               (u8)117   /*G.7221*/
#define   ACTIVE_TYPE_H261	              (u8)118   /*H.261*/
#define   ACTIVE_TYPE_H262	              (u8)119   /*H.262 (MPEG-2)*/
#define   ACTIVE_TYPE_H263	              (u8)120   /*H.263*/
#define   ACTIVE_TYPE_G7221C			  (u8)121	/*G7221c*/
#define   ACTIVE_TYPE_ADPCM				  (u8)122	/*ADPCM*/	
#define   ACTIVE_TYPE_G719                (u8)123   /*G.719*/
	



/*APP ID*/

#define AID_KDV_BASE                      (u16)0 



/*���ܿͻ����ڲ�Ӧ�úţ�1-10��*/

#define AID_NMC_BGN      AID_KDV_BASE + 1

#define AID_NMC_END      AID_NMC_BGN + 9

	

/*�������̨�ڲ�Ӧ�úţ�11-20��*/

#define AID_MCS_BGN      AID_NMC_END + 1

#define AID_MCS_END      AID_MCS_BGN + 9	



/*�ն˿���̨�ڲ�Ӧ�úţ�21-30��*/

#define AID_MTC_BGN      AID_MCS_END + 1

#define AID_MTC_END      AID_MTC_BGN + 9

	

/*���ܷ������ڲ�Ӧ�úţ�31-40��*/

#define AID_SVR_BGN      AID_MTC_END + 1

#define AID_SVR_END      AID_SVR_BGN + 9



/*�����ڲ�Ӧ�úţ�41-50��*/

#define AID_AGT_BGN      AID_SVR_END + 1

#define AID_AGT_END      AID_AGT_BGN + 9

	

/*MCU�ڲ�Ӧ�úţ�51-100��*/

#define AID_MCU_BGN      AID_AGT_END + 1

#define AID_MCU_END       AID_MCU_BGN + 49

	

/*MT�ڲ�Ӧ�úţ�101-150��*/

#define AID_MT_BGN       AID_MCU_END + 1

#define AID_MT_END       AID_MT_BGN + 49

	

/*RECORDER�ڲ�Ӧ�úţ�151-160��*/

#define AID_REC_BGN      AID_MT_END + 1

#define AID_REC_END      AID_REC_BGN + 9

	

/*�������ڲ�Ӧ�úţ�161-170��*/

#define AID_MIXER_BGN    AID_REC_END + 1

#define AID_MIXER_END    AID_MIXER_BGN + 9

	

/*����ǽ�ڲ�Ӧ�úţ�171-181��*/

#define AID_TVWALL_BGN   AID_MIXER_END + 1

#define AID_TVWALL_END   AID_TVWALL_BGN + 9



/*T.120�������ڲ�Ӧ�úţ�181-200��*/

#define AID_DCS_BGN      AID_TVWALL_END + 1

#define AID_DCS_END      AID_DCS_BGN + 19



/*T.120����̨�ڲ�Ӧ�úţ�201-210��*/

#define AID_DCC_BGN      AID_DCS_END + 1

#define AID_DCC_END      AID_DCC_BGN + 9



/*��������������ڲ�Ӧ�úţ�211-220��*/

#define AID_BAS_BGN      AID_DCC_END + 1

#define AID_BAS_END      AID_BAS_BGN + 9



/*GK�������ڲ�Ӧ�ú�(221 - 230)*/

#define AID_GKS_BGN      AID_BAS_END+1

#define AID_GKS_END      AID_GKS_BGN + 9



/*GK����̨�ڲ�Ӧ�ú�(231 - 235)*/

#define AID_GKC_BGN      AID_GKS_END+1

#define AID_GKC_END      AID_GKC_BGN + 4



/*�û������ڲ�Ӧ�ú�(236 - 240)*/

#define AID_UM_BGN      AID_GKC_END+1

#define AID_UM_END      AID_UM_BGN + 4



/*��ַ���ڲ�Ӧ�ú�(241 - 250)*/

#define AID_ADDRBOOK_BGN    AID_UM_END+1

#define AID_ADDRBOOK_END    AID_ADDRBOOK_BGN + 9

/*���ݻ����ն��ڲ�Ӧ�ú�(251 - 260)*/

#define AID_DCMT_BGN    AID_ADDRBOOK_END+1

#define AID_DCMT_END    AID_DCMT_BGN + 9

/*mdsc hdsc watchdog ģ��(261-265) hualiang add*/
#define AID_DSC_BGN    AID_DCMT_END + 1 
#define AID_DSC_END    AID_DSC_BGN + 4

/* radius �Ʒ� ģ��(266-275) guozhongjun add*/
#define  AID_RS_BGN    AID_DSC_END + 1
#define  AID_RS_END    AID_RS_BGN + 9

/* �ļ����������� ģ��(276 - 283) wanghao 2007/1/4 */
#define AID_SUS_BGN             AID_RS_END + 1
#define AID_SUS_END             AID_SUS_BGN + 7

/* �ļ������ͻ��� ģ��(284 - 285) wanghao 2007/1/4 */
#define AID_SUC_BGN             AID_SUS_END + 1
#define AID_SUC_END             AID_SUC_BGN + 1

/* RTSP server ģ��(286 - 290) xsl add */
#define AID_RTSP_BGN			AID_SUC_END +1
#define AID_RTSP_END			AID_RTSP_BGN +5


/*2004/07/15 ��ͬ������Ƶ���ʶ���*/

#define AUDIO_BITRATE_G711A 64
#define AUDIO_BITRATE_G711U 64
#define AUDIO_BITRATE_G7231 6
#define AUDIO_BITRATE_MP3   48
#define AUDIO_BITRATE_G728  16
#define AUDIO_BITRATE_G729  8/*�ݲ�֧��*/
#define AUDIO_BITRATE_G722  64
#define AUDIO_BITRATE_G7221C   24 /* guzh [2008/09/03] ��ֵ�ǲ�׼ȷ�ģ���Ҫ���ݾ�������ȷ�� */
#define AUDIO_BITRATE_G719  24  //lukunpeng [2010/04/15]
#define AUDIO_BITRATE_AAC   96  /* guzh [01/18/2008] */


/*��Ƶ�����Ʒ��ģ��ļ����˿�*/

const u16 PORT_NMS				= 60000;

const u16 PORT_NMC				= 60000;

const u16 PORT_MCU				= 60000;

const u16 PORT_MCS				= 60000;

const u16 PORT_MT				= 60000;

const u16 PORT_MTC				= 60000;

const u16 PORT_TVWALL			= 60000;

const u16 PORT_DIGITALMIXER	    = 60000;

const u16 PORT_RECORDER      	= 60000;

const u16 PORT_DCS				= 61000;

const u16 PORT_GK          		= 60000;

const u16 PORT_GKC         		= 60000; 

const u16 PORT_RS               = 61000;

/*��Ƶ�����Ʒ����Ʒ���Ͷ���*/
const u16 MT_TYPE_KDV7620_B    = 0x042D;
const u16 MT_TYPE_KDV7620_A    = 0x042E;

const u16 MT_TYPE_KDV7820_A    = 0x0431;
const u16 MT_TYPE_KDV7920_A    = 0x0432;

const u16 MT_TYPE_KDV7820_B    = 0x043B;
const u16 MT_TYPE_KDV7920_B    = 0x043C;
	
//��Ʒpid
#define BRD_PID_KDM200_HDU				0x043A
#define BRD_PID_KDM200_HDU_L		    0x0444
#define BRD_PID_KDV_EAPU		        0x0443
#define BRD_PID_KDV_MPU					0x0436
#define BRD_PID_KDV_MAU					0x0439	
#define BRD_PID_KDV8000A_MPC2		    0x043D
#define BRD_PID_KDV8000A_CRI2			0x043E
#define BRD_PID_KDV8000A_DRI2   		0x043F
#define BRD_PID_KDV8000A_IS21   		0x0440
#define BRD_PID_KDV8000A_IS22  			0x0441 
#define KDV8000E						0x0442

#ifdef __cplusplus

}

#endif  /* __cplusplus */



#endif /* _KDV_def_H_ */



/* end of file kdvdef.h */
























