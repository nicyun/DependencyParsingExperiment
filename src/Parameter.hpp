#ifndef _PARAMETER_H_
#define _PARAMETER_H_

/*global variables*/

#define LEARNTIMES	20	/*times of online learning*/
#define BETA		0.2	/*beta for mutation*/
#define DETA		0.1	/*quantity changed by mutation*/

/*Status of Word-agent*/
#define ACTIVE		1	/*active*/
#define MUTATE		2	/*mutate*/
#define CLONE		3	/*clone*/
#define MATURE		4	/*mature*/
#define DIE		6	/*die*/

#define AFFINITY	0.8	/*affinity threshold for adaptive recognition*/
#define STIMULUS	0.8	/*stimulus threshold*/
#define SUPPRESS	0.8	/*suppress threshold*/

#endif
