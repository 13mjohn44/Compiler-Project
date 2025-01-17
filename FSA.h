#include <iostream>
#ifndef FSA_H
#define FSA_H

using namespace std;

enum States{ // states 0-43
    s1,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    s12,
    s13,
    s14,
    s15,
    s16,
    s17,
    s18,
    s19,
    s20,
    s21,
    s22,
    s23,
    s24,
    s25,
    s26,
    s27,
    s28,
    s29,
    s30,
    s31,
    s32,
    s33,
    s34,
    s35,
    s36,
    s37,
    s38,
    s39,
    s40,
    s41,
    s42,
    s43,
    // final states
    ID_TK = 1000,
    INT_TK,
    KW_TK,
    CLN_TK,
    ASN2TK,
    ASN_TK,
    GTR_TK,
    EQV_TK,
    LES_TK,
    PLU_TK,
    SUB_TK,
    MUL_TK,
    DIV_TK,
    EXC_TK,
    DOT_TK,
    LPARTK,
    RPARTK,
    COM_TK,
    LCURTK,
    RCURTK,
    SEM_TK,
    LBRATK,
    RBRATK,
    CRT_TK,
    NEQVTK,
    AND_OP,
    OR__OP,
    EOFTK,
    // error states;
    TLID = -1,//Too Long Identifier
    TLIN = -2,//Too long Int
    NOST = -3,//starting with unacceptable character
    NOCH = -4,//No Character/not valid character
    ADIN = -5,//Interupted and operator token
    INRP = -6,//Interupted while parsing token
    ORIN = -7,//Interupted or operator token

    noState = -1000// no state
    
};  



//curently accepted operators: = == =!= : :=  +  -  *  /  . (  ) , { } ; [ ] < > && || _

//  letter,  caret,  digit,  WS,  colon, greater,equal,lesser, plus, minus,   star,  slash,  !   ,dot,  LParen,RParen,comma,LCurly,RCurly,semi,LBracket,RBrack,  &   ,   |  ,  _   ,  _EOF,  NoChar    
//      0     1       2      3      4      5      6      7      8      9      10     11     12     13     14     15     16     17     18     19     20     21    22      23    24     25     26
const int Table[43][27] = {
    {s2    ,   s38,   s10,    s1,   s18,   s21,   s20,   s23,   s24,   s25,   s26,   s27,  NOCH,   s29,   s30,   s31,   s32,   s33,   s34,   s35,   s36,   s37,   s41,   s28,  NOST, EOFTK,NOCH}, //0
    {s3    , ID_TK,    s3, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,  NOCH, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,    s3, ID_TK,NOCH}, //1
    {s4    , ID_TK,    s4, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,  NOCH, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,    s4, ID_TK,NOCH}, //2
    {s5    , ID_TK,    s5, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,  NOCH, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,    s5, ID_TK,NOCH}, //3
    {s6    , ID_TK,    s6, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,  NOCH, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,    s6, ID_TK,NOCH}, //4
    {s7    , ID_TK,    s7, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,  NOCH, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,    s7, ID_TK,NOCH}, //5
    {s8    , ID_TK,    s8, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,  NOCH, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,    s8, ID_TK,NOCH}, //6
    {s9    , ID_TK,    s9, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,  NOCH, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,    s9, ID_TK,NOCH}, //7
    {TLID  , ID_TK,  TLID, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,  NOCH, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK, ID_TK,  TLID, ID_TK,NOCH}, //8
    {INT_TK,INT_TK,   s11,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,NOCH}, //9
    {INT_TK,INT_TK,   s12,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,NOCH}, //10
    {INT_TK,INT_TK,   s13,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,NOCH}, //11
    {INT_TK,INT_TK,   s14,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,NOCH}, //12
    {INT_TK,INT_TK,   s15,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,NOCH}, //13
    {INT_TK,INT_TK,   s16,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,NOCH}, //14
    {INT_TK,INT_TK,   s17,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,NOCH}, //15
    {INT_TK,INT_TK,  TLIN,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,INT_TK,NOCH}, //16
    {CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,   s19,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,CLN_TK,NOCH}, //17
    {ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,ASN2TK,NOCH}, //18
    {ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,   s22,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,   s39,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,ASN_TK,NOCH}, //19
    {GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,GTR_TK,NOCH}, //20
    {EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,EQV_TK,NOCH}, //21
    {LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,LES_TK,NOCH}, //22
    {PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,PLU_TK,NOCH}, //23
    {SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,SUB_TK,NOCH}, //24
    {MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,MUL_TK,NOCH}, //25
    {DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,DIV_TK,NOCH}, //26
    {  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,  ORIN,   s43,  ORIN,  ORIN,NOCH}, //27
    {DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,DOT_TK,NOCH}, //28
    {LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,LPARTK,NOCH}, //29
    {RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,RPARTK,NOCH}, //30
    {COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,COM_TK,NOCH}, //31
    {LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,LCURTK,NOCH}, //32
    {RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,RCURTK,NOCH}, //33
    {SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,SEM_TK,NOCH}, //34
    {LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,LBRATK,NOCH}, //35
    {RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,RBRATK,NOCH}, //36
    {CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,CRT_TK,NOCH}, //37
    {  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,   s40,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,  INRP,NOCH}, //38
    {NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NEQVTK,NOCH}, //39
    {  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,  ADIN,   s42,  ADIN,  ADIN,  ADIN,NOCH}, //40
    {AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,AND_OP,NOCH}, //41
    {OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,OR__OP,NOCH}  //42
};

#endif