
#include <common.h>
#include <rte_cycles.h>
uint8_t tw_convert_str_to_iparr(const char *value, uint8_t *ipVar) {
    int ip[5];
    ip[0] = -1;
    ip[4] = tw_str_len(value);
    tw_find_char(value, &ip[1], '.', 3);

    if (ip == NULL)
        return 0;
    else {
        uint8_t n = 0;
        for (n = 0; n < 4; n++) {
            char temp[3]={'\0',
                          '\0',
                          '\0'};

            if (((ip[n + 1]) - (ip[n] + 1)) == 1) {
                temp[0] = '0';
                temp[1] = '0';
                temp[2] = value[ip[n + 1] - 1];
            } else if (((ip[n + 1]) - (ip[n] + 1)) == 2) {
                temp[0] = '0';
                temp[1] = value[ip[n + 1] - 2];
                temp[2] = value[ip[n + 1] - 1];
            } else if (((ip[n + 1]) - (ip[n] + 1)) == 3) {
                temp[0] = value[ip[n + 1] - 3];
                temp[1] = value[ip[n + 1] - 2];
                temp[2] = value[ip[n + 1] - 1];
            } else if ((((ip[n + 1]) - (ip[n] + 1)) > 3) || (((ip[n + 1]) - (ip[n] + 1)) < 1))
                return 0;

            ipVar[n] = tw_convert_str_to_int(temp, 3);
        }
        return 1;
    }
}

uint32_t tw_convert_str_to_hex(char * str, uint8_t length) {
    uint32_t result = 0;
    uint8_t elements[length];
    int n;
    for (n = length - 1; n >= 0; n--) {
        if (*(str + n) > '9') {
            elements[n] = *(str + n) - 87;
            result += (elements[n] << (4 * (length - n - 1)));
            continue;
        }

        elements[n] = *(str + n) - 48;
        result += (elements[n] << (4 * (length - n - 1)));
    }
    return result;
}

unsigned short tw_calcsum(unsigned short *buffer, int length){
	unsigned long sum;
	// initialize sum to zero and loop until length (in words) is 0
	for (sum=0; length>1; length-=2) // sizeof() returns number of bytes, we're interested in number of words
		sum += *buffer++;	// add 1 word of buffer to sum and proceed to the next

	// we may have an extra byte
	if (length==1)
		sum += (char)*buffer;

	sum = (sum >> 16) + (sum & 0xFFFF);  // add high 16 to low 16
	sum += (sum >> 16);		     // add carry
	return ~sum;
}

void tw_ip_dec_to_arr(uint32_t ip, uint8_t *arr) {
    uint32_t temp = ip;
    arr[0] = (uint8_t) (temp >> 24);
    arr[1] = (uint8_t) (temp >> 16);
    arr[2] = (uint8_t) (temp >> 8);
    arr[3] = (uint8_t) (temp);
}

uint32_t tw_ip_arr_to_dec(uint8_t *ip) {
    return ( (uint32_t) ((ip[0] << 24) + (ip[1] << 16) + (ip[2] << 8) + (ip[3])));
}

uint32_t tw_convert_ip_str_to_dec(const char * ip_str) {
    uint8_t ip_arr[4];
    if (tw_convert_str_to_iparr(ip_str, ip_arr))
        return tw_ip_arr_to_dec(ip_arr);
    return 0; //--? rte_exit or not?
}

uint8_t tw_str_len(const char *str) {
    int size = -1;

    while (str[size + 1] != '\0')
        size = size + 1;

    return size + 1;
}

void tw_slice_array(const char *srcStr, char *dstStr, uint8_t start, uint8_t dstLength) {
    uint8_t count = 0;

    for (count = 0; count < dstLength; count++) {
        dstStr[count] = srcStr[count + start];
    }

    dstStr[dstLength] = '\0';
}

uint8_t tw_find_char(const char *str, int *quoteLoc, char c, uint8_t instances) {
    uint8_t qCount = 0;
    uint8_t i = 0;

    for (i = 0; i < tw_str_len(str); i++) {
        if (str[i] == c && qCount < instances)// as there are only 4 quotes present in a line
        {
            quoteLoc[qCount] = i;
            qCount++;
        }
    }

    if (qCount != instances) {
        quoteLoc = NULL;
        return 0; //--? shouldn't it be NULL
    }
    return qCount;
}

uint64_t tw_convert_str_to_int(const char *str, uint8_t n) {
    uint8_t count = 0;
    uint64_t num = 0;

    for (count = n; count > 0; count--)
        num = num + ((str[count - 1] - 48) * tw_power(10, (n - 1 - (count - 1))));
    return num;
}

int tw_power(uint8_t base, uint8_t exp) {
    if (exp == 0)
        return 1;
    else
        return ( base * tw_power(base, exp - 1));
}

inline uint16_t tw_be_to_cpu_16(uint16_t value) {
    return rte_be_to_cpu_16(value);
}

inline uint16_t tw_cpu_to_be_16(uint16_t value) {
    return rte_cpu_to_be_16(value);
}

inline uint32_t tw_cpu_to_be_32(uint32_t value) {
    return rte_cpu_to_be_32(value);
}

inline uint32_t tw_be_to_cpu_32(uint32_t value) {
    return rte_be_to_cpu_32(value);
}

inline uint64_t tw_get_tsc_hz(void) {
	return rte_get_tsc_hz();
}

uint8_t tw_parse_int_from_string(char* str) {
    
    char *p = str;
    uint8_t val=0; 
    while (*p) {
        if (isdigit(*p)) {
            val = strtol(p, &p, 10);
        } 
        else {
            p++;
        }
    }
    return val;

}

