
#include "sigar_format.h"
#include <arpa/inet.h>

#if defined(SIGAR_USING_MSC6)
#define sigar_inet_ntop(af, src, dst, size) NULL
#define sigar_inet_ntop_errno SIGAR_ENOTIMPL
#elif defined(WIN32)
static char *sigar_inet_ntop(int af, const void *src, char *dst, int cnt)
{
    struct sockaddr_in6 sa; /* note only using this for AF_INET6 */

    memset(&sa, '\0', sizeof(sa));
    sa.sin6_family = af;
    memcpy(&sa.sin6_addr, src, sizeof(sa.sin6_addr));

    if (getnameinfo((struct sockaddr *)&sa, sizeof(sa),
                    dst, cnt, NULL, 0, NI_NUMERICHOST))
    {
        return NULL;
    }
    else {
        return dst;
    }
}
#define sigar_inet_ntop_errno GetLastError()
#else
#define sigar_inet_ntop inet_ntop
#define sigar_inet_ntop_errno errno
#endif

int sigar_inet_ntoa(sigar_t *sigar,
                    sigar_uint32_t address,
                    char *addr_str)
{
    char *next=addr_str;
    int n=0;
    const unsigned char *src =
        (const unsigned char *)&address;

    do {
        unsigned char u = *src++;
        if (u > 99) {
            *next++ = '0' + u/100;
            u %= 100;
            *next++ = '0' + u/10;
            u %= 10;
        }
        else if (u > 9) {
            *next++ = '0' + u/10;
            u %= 10;
        }
        *next++ = '0' + u;
        *next++ = '.';
        n++;
    } while (n < 4);

    *--next = 0;

    return SIGAR_OK;
}

static int test_sigar_ether_ntoa(char *buff, unsigned char *ptr)
{
    sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X",
            (ptr[0] & 0xff), (ptr[1] & 0xff), (ptr[2] & 0xff),
            (ptr[3] & 0xff), (ptr[4] & 0xff), (ptr[5] & 0xff));
    return SIGAR_OK;
}


SIGAR_DECLARE(int) sigar_net_address_to_string(sigar_t *sigar, sigar_net_address_t *address, char *addr_str)
{
    *addr_str = '\0';
    switch (address->family) {
      case sigar_net_address_t::SIGAR_AF_INET6:
		  if (sigar_inet_ntop(AF_INET6, (const void *)&address->addr.in6,
							  addr_str, SIGAR_INET6_ADDRSTRLEN))
		  {
			  return SIGAR_OK;
		  }
		  else {
			  return sigar_inet_ntop_errno;
		  }
      case sigar_net_address_t::SIGAR_AF_INET:
        return sigar_inet_ntoa(sigar, address->addr.in, addr_str);
      case sigar_net_address_t::SIGAR_AF_UNSPEC:
        return sigar_inet_ntoa(sigar, 0, addr_str); /*XXX*/
      case sigar_net_address_t::SIGAR_AF_LINK:
        return test_sigar_ether_ntoa(addr_str, &address->addr.mac[0]);
      default:
        return EINVAL;
    }
}


SIGAR_DECLARE(char *) sigar_format_size(sigar_uint64_t size, char *buf)
{
    const char ord[] = "KMGTPE";
    const char *o = ord;
    int remain;

    if (size == SIGAR_FIELD_NOTIMPL) {
        buf[0] = '-';
        buf[1] = '\0';
        return buf;
    }

    if (size < 973) {
        sprintf(buf, "%3d ", (int) size);
        return buf;
    }

    do {
        remain = (int)(size & 1023);
        size >>= 10;

        if (size >= 973) {
            ++o;
            continue;
        }

        if (size < 9 || (size == 9 && remain < 973)) {
            if ((remain = ((remain * 5) + 256) / 512) >= 10) {
                ++size;
                remain = 0;
            }
            sprintf(buf, "%d.%d%c", (int) size, remain, *o);
            return buf;
        }

        if (remain >= 512) {
            ++size;
        }

        sprintf(buf, "%3d%c", (int) size, *o);

        return buf;
    } while (1);
}
