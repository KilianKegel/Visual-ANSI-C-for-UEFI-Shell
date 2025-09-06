#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <uefi.h>
#include <protocol\shell.h>
#include <intrin.h>

// Available functions: https://github.com/KilianKegel/toro-C-Library?tab=readme-ov-file#implementation-status--cross-matrix

int rdrtc(int i) {
    __outbyte(0x70, i);
    return __inbyte(0x71);
}

static int bcd2int(int bcd) {
    int h, l;

    h = 0x0F & (bcd / 16);
    l = 0x0F & (bcd);
    return h * 10 + l;
}

int main(int argc, char** argv) {
    EFI_SYSTEM_TABLE* SystemTable = (EFI_SYSTEM_TABLE*)(argv[-1]);      // SystemTable is passed in argv[-1]
    EFI_HANDLE ImageHandle = (void*)(argv[-2]);                         // ImageHandle is passed in argv[-2]
    int nRet = 0;
    struct tm Tm, * pTm = &Tm;
    time_t t;
    int oldsec = -1, num = 0;
    int rtcsec, rtcmin, rtchour, rtcmday, rtcmon, rtcyear;
    unsigned long long TSCTsc, RTCTsc;

    printf("Demonstration of the accuracy of TSC-based 'TIME.H' functions\n");
    printf("NOTE: This program will crash on Windows due to direct RTC access.\n");
    printf("%ls -> %X, %d.%d\n",
        SystemTable->FirmwareVendor,
        SystemTable->FirmwareRevision,
        SystemTable->Hdr.Revision >> 16,
        SystemTable->Hdr.Revision & 0xFFFF);

    do {
        t = time(NULL);                                 // Get system time
        pTm = localtime(&t);
        TSCTsc = __rdtsc();

        if (oldsec != pTm->tm_sec) {
            oldsec = pTm->tm_sec;

            if (0x80 & rdrtc(0x0A))
                continue;                               // Skip if update cycle is in progress
            rtcyear = bcd2int(rdrtc(9));
            rtcyear += (100 * bcd2int(rdrtc(0x32)));
            rtcmon = bcd2int(rdrtc(8));
            rtcmday = bcd2int(rdrtc(7));
            rtchour = bcd2int(rdrtc(4));
            rtcmin = bcd2int(rdrtc(2));
            rtcsec = bcd2int(rdrtc(0));

            while (0 == (0x80 & rdrtc(0x0A)))           // Wait for update cycle
                ;
            RTCTsc = __rdtsc();                         // Get time difference RTCTsc vs. TSCTsc

            // Perform printf() during the update cycle
            printf("TSCTime: %d/%02d/%02d %02d:%02d,%02d vs. RTCTime: %04d/%02d/%02d %02d:%02d,%02d -> %02d:%02d:%02d sec, %lld, secdiff %d\n",
                pTm->tm_year + 1900,
                pTm->tm_mon + 1,
                pTm->tm_mday,
                pTm->tm_hour,
                pTm->tm_min,
                pTm->tm_sec,
                rtcyear,
                rtcmon,
                rtcmday,
                rtchour,
                rtcmin,
                rtcsec,
                ++num / 3600,
                (num % 3600) / 60,
                (num % 3600) % 60,
                RTCTsc - TSCTsc,
                rtcsec - pTm->tm_sec
            );
        }
    } while (1);

    return nRet;
}
