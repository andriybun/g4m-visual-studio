#include "forest.h"

void woodProductionIndexes(void)
{

for (int i=0;i<NumberOfCountries;i++){
//countryNwp[i]=0;
countryLosses[i]=0.15;
FM_sink_stat[i]=0.;
}

for (int i=0;i<=10;i++){
yearNwp[i]=0;
}

//yearNwp[	0	]=	1990	;
//yearNwp[	1	]=	2000	;
//yearNwp[	2	]=	2005	;
//yearNwp[	3	]=	2010	;
//yearNwp[	4	]=	2020	;
//yearNwp[	5	]=	2030	;
//yearNwp[	6	]=	2050	;


yearNwp[	0	]=	2000	;
yearNwp[	1	]=	2010	;
yearNwp[	2	]=	2020	;
yearNwp[	3	]=	2030	;
yearNwp[	4	]=	2040	;
yearNwp[	5	]=	2050	;
yearNwp[	6	]=	2060	;
yearNwp[	7	]=	2070	;
yearNwp[	8	]=	2080	;
yearNwp[	9	]=	2090	;
yearNwp[	10	]=	2100	;

//country specific harvest losses NEW Country Codes!!!! Shifted by -1 to start array from 0
//Source: Forest Resources of Europe; CIS; North America; Australia; Japan and New Zealand: Main Report 2000
// Estimated by Hannes Bottcher??

countryLosses[	1	]=	0.15	;
countryLosses[	2	]=	0.15	;
countryLosses[	5	]=	0.15	;
countryLosses[	8	]=	0.15	;
countryLosses[	9	]=	0.15	;
countryLosses[	10	]=	0.15	;
countryLosses[	15	]=	0.15	;
countryLosses[	16	]=	0.13	;
countryLosses[	17	]=	0.15	;
countryLosses[	18	]=	0.15	;
countryLosses[	19	]=	0.15	;
countryLosses[	20	]=	0.15	;
countryLosses[	21	]=	0.15	;
countryLosses[	22	]=	0.15	;
countryLosses[	23	]=	0.32	;
countryLosses[	25	]=	0.15	;
countryLosses[	26	]=	0.15	;
countryLosses[	27	]=	0.15	;
countryLosses[	28	]=	0.15	;
countryLosses[	30	]=	0.15	;
countryLosses[	31	]=	0.15	;
countryLosses[	32	]=	0.15	;
countryLosses[	33	]=	0.15	;
countryLosses[	34	]=	0.15	;
countryLosses[	36	]=	0.15	;
countryLosses[	37	]=	0.15	;
countryLosses[	38	]=	0.15	;
countryLosses[	40	]=	0.14	;
countryLosses[	41	]=	0.15	;
countryLosses[	42	]=	0.15	;
countryLosses[	43	]=	0.15	;
countryLosses[	44	]=	0.15	;
countryLosses[	45	]=	0.15	;
countryLosses[	46	]=	0.15	;
countryLosses[	48	]=	0.15	;
countryLosses[	49	]=	0.15	;
countryLosses[	51	]=	0.15	;
countryLosses[	52	]=	0.15	;
countryLosses[	55	]=	0.15	;
countryLosses[	56	]=	0.2	;
countryLosses[	57	]=	0.2	;
countryLosses[	59	]=	0.15	;
countryLosses[	60	]=	0.15	;
countryLosses[	61	]=	0.15	;
countryLosses[	62	]=	0.15	;
countryLosses[	63	]=	0.15	;
countryLosses[	65	]=	0.15	;
countryLosses[	67	]=	0.15	;
countryLosses[	68	]=	0.15	;
countryLosses[	69	]=	0.15	;
countryLosses[	70	]=	0.09	;
countryLosses[	71	]=	0.15	;
countryLosses[	73	]=	0.21	;
countryLosses[	76	]=	0.15	;
countryLosses[	76	]=	0.15	;
countryLosses[	77	]=	0.14	;
countryLosses[	78	]=	0.15	;
countryLosses[	79	]=	0.15	;
countryLosses[	81	]=	0.15	;
countryLosses[	81	]=	0.15	;
countryLosses[	86	]=	0.15	;
countryLosses[	87	]=	0.15	;
countryLosses[	89	]=	0.15	;
countryLosses[	92	]=	0.15	;
countryLosses[	95	]=	0.15	;
countryLosses[	96	]=	0.07	;
countryLosses[	97	]=	0.15	;
countryLosses[	98	]=	0.12	;
countryLosses[	99	]=	0.15	;
countryLosses[	100	]=	0.15	;
countryLosses[	102	]=	0.15	;
countryLosses[	103	]=	0.15	;
countryLosses[	104	]=	0.15	;
countryLosses[	106	]=	0.15	;
countryLosses[	107	]=	0.04	;
countryLosses[	108	]=	0.15	;
countryLosses[	110	]=	0.15	;
countryLosses[	111	]=	0.15	;
countryLosses[	112	]=	0.15	;
countryLosses[	113	]=	0.15	;
countryLosses[	114	]=	0.15	;
countryLosses[	117	]=	0.15	;
countryLosses[	117	]=	0.15	;
countryLosses[	118	]=	0.15	;
countryLosses[	119	]=	0.15	;
countryLosses[	120	]=	0.15	;
countryLosses[	121	]=	0.15	;
countryLosses[	122	]=	0.15	;
countryLosses[	123	]=	0.15	;
countryLosses[	125	]=	0.15	;
countryLosses[	126	]=	0.15	;
countryLosses[	127	]=	0.15	;
countryLosses[	128	]=	0.15	;
countryLosses[	129	]=	0.17	;
countryLosses[	131	]=	0.15	;
countryLosses[	133	]=	0.15	;
countryLosses[	134	]=	0.15	;
countryLosses[	136	]=	0.15	;
countryLosses[	138	]=	0.15	;
countryLosses[	139	]=	0.15	;
countryLosses[	143	]=	0.15	;
countryLosses[	145	]=	0.15	;
countryLosses[	145	]=	0.15	;
countryLosses[	148	]=	0.15	;
countryLosses[	150	]=	0.15	;
countryLosses[	151	]=	0.15	;
countryLosses[	153	]=	0.15	;
countryLosses[	157	]=	0.15	;
countryLosses[	158	]=	0.15	;
countryLosses[	160	]=	0.15	;
countryLosses[	161	]=	0.06	;
countryLosses[	162	]=	0.15	;
countryLosses[	164	]=	0.15	;
countryLosses[	165	]=	0.15	;
countryLosses[	166	]=	0.15	;
countryLosses[	167	]=	0.15	;
countryLosses[	169	]=	0.15	;
countryLosses[	170	]=	0.15	;
countryLosses[	172	]=	0.15	;
countryLosses[	173	]=	0.17	;
countryLosses[	176	]=	0.02	;
countryLosses[	177	]=	0.15	;
countryLosses[	182	]=	0.15	;
countryLosses[	183	]=	0.15	;
countryLosses[	184	]=	0.15	;
countryLosses[	184	]=	0.15	;
countryLosses[	185	]=	0.15	;
countryLosses[	186	]=	0.15	;
countryLosses[	187	]=	0.15	;
countryLosses[	188	]=	0.15	;
countryLosses[	192	]=	0.15	;
countryLosses[	193	]=	0.15	;
countryLosses[	194	]=	0.15	;
countryLosses[	194	]=	0.15	;
countryLosses[	196	]=	0.15	;
countryLosses[	198	]=	0.15	;
countryLosses[	199	]=	0.15	;
countryLosses[	200	]=	0.15	;
countryLosses[	201	]=	0.24	;
countryLosses[	202	]=	0.15	;
countryLosses[	203	]=	0.07	;
countryLosses[	204	]=	0.15	;
countryLosses[	205	]=	0.15	;
countryLosses[	206	]=	0.15	;
countryLosses[	208	]=	0.15	;
countryLosses[	209	]=	0.15	;
countryLosses[	209	]=	0.15	;
countryLosses[	210	]=	0.15	;
countryLosses[	211	]=	0.15	;
countryLosses[	211	]=	0.15	;
countryLosses[	216	]=	0.15	;
countryLosses[	217	]=	0.15	;
countryLosses[	218	]=	0.07	;
countryLosses[	222	]=	0.15	;
countryLosses[	223	]=	0.15	;
countryLosses[	225	]=	0.15	;
countryLosses[	226	]=	0.14	;
countryLosses[	227	]=	0.15	;
countryLosses[	229	]=	0.15	;
countryLosses[	230	]=	0.15	;
countryLosses[	233	]=	0.15	;
countryLosses[	234	]=	0.15	;
countryLosses[	237	]=	0.15	;
countryLosses[	238	]=	0.15	;
countryLosses[	239	]=	0.15	;
countryLosses[	240	]=	0.15	;


//*****************************************************************************
// Forest management CO2 sink (only CO2, only biomass) reported by the countries, GgCO2
//Net stock changes biomass FL-FL (from CRF table 5.A), average for 1990-2000, GgCO2/year ?
//*****************************************************************************
//New country code!!! shifted by -1
FM_sink_stat[	15	]=	39242	;
FM_sink_stat[	16	]=	15001	;
FM_sink_stat[	19	]=	33870	;
FM_sink_stat[	23	]=	2175	;
FM_sink_stat[	27	]=	13607	;
FM_sink_stat[	38	]=	48346	;
FM_sink_stat[	96	]=	8707	;
FM_sink_stat[	56	]=	8731	;
FM_sink_stat[	60	]=	939	;
FM_sink_stat[	68	]=	6424	;
FM_sink_stat[	70	]=	25384	;
FM_sink_stat[	73	]=	52784	;
FM_sink_stat[	57	]=	62429	;
FM_sink_stat[	86	]=	1788	;
FM_sink_stat[	98	]=	4032	;
FM_sink_stat[	105	]=	3	;
FM_sink_stat[	102	]=	604	;
FM_sink_stat[	107	]=	24658	;
FM_sink_stat[	110	]=	80579	;
FM_sink_stat[	129	]=	22899	;
FM_sink_stat[	124	]=	19	;
FM_sink_stat[	127	]=	13654	;
FM_sink_stat[	128	]=	296	;
FM_sink_stat[	132	]=	0	;
FM_sink_stat[	160	]=	2420	;
FM_sink_stat[	164	]=	23387	;
FM_sink_stat[	161	]=	9402	;
FM_sink_stat[	173	]=	24987	;
FM_sink_stat[	176	]=	3630	;
FM_sink_stat[	182	]=	38417	;
FM_sink_stat[	183	]=	235445	;
FM_sink_stat[	201	]=	2000	;
FM_sink_stat[	202	]=	9522	;
FM_sink_stat[	67	]=	39167	;
FM_sink_stat[	203	]=	37819	;
FM_sink_stat[	40	]=	2420	;
FM_sink_stat[	218	]=	44338	;
FM_sink_stat[	223	]=	58968	;
FM_sink_stat[	77	]=	7652	;
FM_sink_stat[	226	]=	434294	;


//******************************************************************************
// country code and Poles region code
//******************************************************************************
// New country codes!!!
coutryRegion[0] = 0 ;	
coutryRegion[	1	]=	0	;
coutryRegion[	2	]=	42	;
coutryRegion[	3	]=	47	;
coutryRegion[	4	]=	0	;
coutryRegion[	5	]=	0	;
coutryRegion[	6	]=	39	;
coutryRegion[	7	]=	0	;
coutryRegion[	8	]=	38	;
coutryRegion[	9	]=	36	;
coutryRegion[	10	]=	41	;
coutryRegion[	11	]=	32	;
coutryRegion[	12	]=	0	;
coutryRegion[	13	]=	0	;
coutryRegion[	14	]=	0	;
coutryRegion[	15	]=	0	;
coutryRegion[	16	]=	28	;
coutryRegion[	17	]=	1	;
coutryRegion[	18	]=	32	;
coutryRegion[	19	]=	47	;
coutryRegion[	20	]=	2	;
coutryRegion[	21	]=	47	;
coutryRegion[	22	]=	47	;
coutryRegion[	23	]=	42	;
coutryRegion[	24	]=	3	;
coutryRegion[	25	]=	36	;
coutryRegion[	26	]=	38	;
coutryRegion[	27	]=	39	;
coutryRegion[	28	]=	32	;
coutryRegion[	29	]=	38	;
coutryRegion[	30	]=	38	;
coutryRegion[	31	]=	41	;
coutryRegion[	32	]=	29	;
coutryRegion[	33	]=	38	;
coutryRegion[	34	]=	43	;
coutryRegion[	35	]=	42	;
coutryRegion[	36	]=	0	;
coutryRegion[	37	]=	47	;
coutryRegion[	38	]=	47	;
coutryRegion[	39	]=	30	;
coutryRegion[	40	]=	0	;
coutryRegion[	41	]=	40	;
coutryRegion[	42	]=	41	;
coutryRegion[	43	]=	31	;
coutryRegion[	44	]=	47	;
coutryRegion[	45	]=	47	;
coutryRegion[	46	]=	47	;
coutryRegion[	47	]=	47	;
coutryRegion[	48	]=	0	;
coutryRegion[	49	]=	41	;
coutryRegion[	50	]=	47	;
coutryRegion[	51	]=	47	;
coutryRegion[	52	]=	38	;
coutryRegion[	53	]=	38	;
coutryRegion[	54	]=	0	;
coutryRegion[	55	]=	0	;
coutryRegion[	56	]=	4	;
coutryRegion[	57	]=	5	;
coutryRegion[	58	]=	10	;
coutryRegion[	59	]=	47	;
coutryRegion[	60	]=	38	;
coutryRegion[	61	]=	6	;
coutryRegion[	62	]=	38	;
coutryRegion[	63	]=	36	;
coutryRegion[	64	]=	41	;
coutryRegion[	65	]=	36	;
coutryRegion[	66	]=	47	;
coutryRegion[	67	]=	0	;
coutryRegion[	68	]=	25	;
coutryRegion[	69	]=	7	;
coutryRegion[	70	]=	47	;
coutryRegion[	71	]=	8	;
coutryRegion[	72	]=	37	;
coutryRegion[	73	]=	0	;
coutryRegion[	74	]=	9	;
coutryRegion[	75	]=	0	;
coutryRegion[	76	]=	0	;
coutryRegion[	77	]=	47	;
coutryRegion[	78	]=	27	;
coutryRegion[	79	]=	32	;
coutryRegion[	80	]=	47	;
coutryRegion[	81	]=	40	;
coutryRegion[	82	]=	47	;
coutryRegion[	83	]=	0	;
coutryRegion[	84	]=	47	;
coutryRegion[	85	]=	47	;
coutryRegion[	86	]=	0	;
coutryRegion[	87	]=	11	;
coutryRegion[	88	]=	38	;
coutryRegion[	89	]=	0	;
coutryRegion[	90	]=	38	;
coutryRegion[	91	]=	0	;
coutryRegion[	92	]=	0	;
coutryRegion[	93	]=	41	;
coutryRegion[	94	]=	0	;
coutryRegion[	95	]=	0	;
coutryRegion[	96	]=	38	;
coutryRegion[	97	]=	39	;
coutryRegion[	98	]=	38	;
coutryRegion[	99	]=	12	;
coutryRegion[	100	]=	43	;
coutryRegion[	101	]=	33	;
coutryRegion[	102	]=	0	;
coutryRegion[	103	]=	13	;
coutryRegion[	104	]=	36	;
coutryRegion[	105	]=	36	;
coutryRegion[	106	]=	40	;
coutryRegion[	107	]=	36	;
coutryRegion[	108	]=	14	;
coutryRegion[	109	]=	38	;
coutryRegion[	110	]=	36	;
coutryRegion[	111	]=	34	;
coutryRegion[	112	]=	32	;
coutryRegion[	113	]=	47	;
coutryRegion[	114	]=	32	;
coutryRegion[	115	]=	44	;
coutryRegion[	116	]=	37	;
coutryRegion[	117	]=	0	;
coutryRegion[	118	]=	46	;
coutryRegion[	119	]=	36	;
coutryRegion[	120	]=	44	;
coutryRegion[	121	]=	36	;
coutryRegion[	122	]=	47	;
coutryRegion[	123	]=	36	;
coutryRegion[	124	]=	38	;
coutryRegion[	125	]=	0	;
coutryRegion[	126	]=	42	;
coutryRegion[	127	]=	47	;
coutryRegion[	128	]=	16	;
coutryRegion[	129	]=	17	;
coutryRegion[	130	]=	15	;
coutryRegion[	131	]=	0	;
coutryRegion[	132	]=	36	;
coutryRegion[	133	]=	0	;
coutryRegion[	134	]=	32	;
coutryRegion[	135	]=	47	;
coutryRegion[	136	]=	42	;
coutryRegion[	137	]=	35	;
coutryRegion[	138	]=	0	;
coutryRegion[	139	]=	39	;
coutryRegion[	140	]=	47	;
coutryRegion[	141	]=	18	;
coutryRegion[	142	]=	43	;
coutryRegion[	143	]=	39	;
coutryRegion[	144	]=	44	;
coutryRegion[	145	]=	0	;
coutryRegion[	146	]=	47	;
coutryRegion[	147	]=	47	;
coutryRegion[	148	]=	0	;
coutryRegion[	149	]=	47	;
coutryRegion[	150	]=	0	;
coutryRegion[	151	]=	47	;
coutryRegion[	152	]=	43	;
coutryRegion[	153	]=	0	;
coutryRegion[	154	]=	47	;
coutryRegion[	155	]=	0	;
coutryRegion[	156	]=	47	;
coutryRegion[	157	]=	0	;
coutryRegion[	158	]=	47	;
coutryRegion[	159	]=	38	;
coutryRegion[	160	]=	0	;
coutryRegion[	161	]=	19	;
coutryRegion[	162	]=	40	;
coutryRegion[	163	]=	42	;
coutryRegion[	164	]=	0	;
coutryRegion[	165	]=	28	;
coutryRegion[	166	]=	36	;
coutryRegion[	167	]=	42	;
coutryRegion[	168	]=	38	;
coutryRegion[	169	]=	0	;
coutryRegion[	170	]=	41	;
coutryRegion[	171	]=	43	;
coutryRegion[	172	]=	0	;
coutryRegion[	173	]=	37	;
coutryRegion[	174	]=	20	;
coutryRegion[	175	]=	0	;
coutryRegion[	176	]=	44	;
coutryRegion[	177	]=	21	;
coutryRegion[	178	]=	41	;
coutryRegion[	179	]=	0	;
coutryRegion[	180	]=	0	;
coutryRegion[	181	]=	36	;
coutryRegion[	182	]=	0	;
coutryRegion[	183	]=	22	;
coutryRegion[	184	]=	32	;
coutryRegion[	185	]=	47	;
coutryRegion[	186	]=	36	;
coutryRegion[	187	]=	47	;
coutryRegion[	188	]=	47	;
coutryRegion[	189	]=	43	;
coutryRegion[	190	]=	0	;
coutryRegion[	191	]=	0	;
coutryRegion[	192	]=	0	;
coutryRegion[	193	]=	37	;
coutryRegion[	194	]=	47	;
coutryRegion[	195	]=	47	;
coutryRegion[	196	]=	0	;
coutryRegion[	197	]=	47	;
coutryRegion[	198	]=	0	;
coutryRegion[	199	]=	39	;
coutryRegion[	200	]=	47	;
coutryRegion[	201	]=	41	;
coutryRegion[	202	]=	23	;
coutryRegion[	203	]=	24	;
coutryRegion[	204	]=	26	;
coutryRegion[	205	]=	47	;
coutryRegion[	206	]=	47	;
coutryRegion[	207	]=	36	;
coutryRegion[	208	]=	0	;
coutryRegion[	209	]=	47	;
coutryRegion[	210	]=	47	;
coutryRegion[	211	]=	43	;
coutryRegion[	212	]=	32	;
coutryRegion[	213	]=	0	;
coutryRegion[	214	]=	32	;
coutryRegion[	215	]=	0	;
coutryRegion[	216	]=	0	;
coutryRegion[	217	]=	38	;
coutryRegion[	218	]=	36	;
coutryRegion[	219	]=	48	;
coutryRegion[	220	]=	0	;
coutryRegion[	221	]=	36	;
coutryRegion[	222	]=	47	;
coutryRegion[	223	]=	47	;
coutryRegion[	224	]=	32	;
coutryRegion[	225	]=	0	;
coutryRegion[	226	]=	41	;
coutryRegion[	227	]=	49	;
coutryRegion[	228	]=	32	;
coutryRegion[	229	]=	0	;
coutryRegion[	230	]=	38	;
coutryRegion[	231	]=	41	;
coutryRegion[	232	]=	0	;
coutryRegion[	233	]=	0	;
coutryRegion[	234	]=	44	;
coutryRegion[	235	]=	37	;
coutryRegion[	236	]=	0	;
coutryRegion[	237	]=	0	;
coutryRegion[	238	]=	36	;
coutryRegion[	239	]=	45	;
coutryRegion[	240	]=	47	;
coutryRegion[	241	]=	47	;
coutryRegion[	242	]=	0	;
coutryRegion[	243	]=	0	;
coutryRegion[	244	]=	0	;


}
