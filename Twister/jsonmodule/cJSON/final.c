#include "jsontobeinclude.h"


int main (int argc, const char * argv[]) {
	
	/* Parse standard testfiles: */
	//dofile("/home/monster/Documents/cJSONFiles/cJSON/tests/jsontemplate");
    char *rawjson = registerfile("/home/monster/Documents/cJSONFiles/cJSON/tests/jsontemplate"); 
	//printf("%s", path) ;
	
	cJSON *jsonconfigfile, *ips, *ip2;
	char *out;
	FILE *f;
	long len;
	char *data;
	int i,j;
	cJSON* name = NULL;
    cJSON* index = NULL;
    cJSON* optional = NULL;
	
	
	jsonconfigfile=cJSON_Parse(rawjson);

	if (!jsonconfigfile) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	
	else
	{   
		out=cJSON_Print(jsonconfigfile);
		printf("%s\n",out);
		
		
		for (i = 0 ; i < cJSON_GetArraySize(jsonconfigfile) ; i++)
		{
		    cJSON * subitem = cJSON_GetArrayItem(jsonconfigfile, i);
            
            
            char*  portnum= cJSON_GetObjectItem(subitem, "portnum")->valuestring;
            if ( !strncmp(portnum,"01",2) )
            {
             printf ("kkkkkkk\n");
             printf("portnum=%s\n", portnum);
             cJSON * ips= cJSON_GetObjectItem(subitem, "ips");
              for (j = 0 ; j < cJSON_GetArraySize(ips) ; j++)
		    
		    {
		        cJSON * subdictip = cJSON_GetArrayItem(ips, j);
		        char* ipaddress = cJSON_GetObjectItem(subdictip, "ip")->valuestring;
                char*  range= cJSON_GetObjectItem(subdictip, "range")->valuestring;
                printf("ipaddress==%s\n" , ipaddress);
                printf("range==%s\n" , range);
            }
             
             
             
            }
            
            printf("portnum=%s\n", portnum);
            
            
		}
		
	
    
    
    }
    
    //fp = fopen( "test_summary" , "a" );
	//fprintf(fp, "test stats successfully saved to file\n");
	//fprintf(fp, "%s\n", out);
	//fclose(fp);
	//free(out);
    


	
	
    
	/* Now some samplecode for building objects concisely: */
	//create_objects();
	
	return 0;
}
