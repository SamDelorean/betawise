#include "betawise.h"

#define DEFINE_SYSCALL(addr, name) \
    asm(".global " #name); \
    asm(#name ": .word " #addr)

DEFINE_SYSCALL(0xA000, ClearScreen);
DEFINE_SYSCALL(0xA004, SetCursor);
DEFINE_SYSCALL(0xA008, GetCursorPos);
DEFINE_SYSCALL(0xA00C, PutStringCentered);
DEFINE_SYSCALL(0xA010, PutChar);
DEFINE_SYSCALL(0xA014, PutStringRaw);
DEFINE_SYSCALL(0xA018, SYS_A018);
DEFINE_SYSCALL(0xA01C, SYS_A01C);
DEFINE_SYSCALL(0xA020, SYS_A020);
DEFINE_SYSCALL(0xA024, SYS_A024);
DEFINE_SYSCALL(0xA028, SYS_A028);
DEFINE_SYSCALL(0xA02C, SYS_A02C);
DEFINE_SYSCALL(0xA030, PutString);
DEFINE_SYSCALL(0xA034, SYS_A034);
DEFINE_SYSCALL(0xA038, SYS_A038);
DEFINE_SYSCALL(0xA03C, SYS_A03C);
DEFINE_SYSCALL(0xA040, SYS_A040);
DEFINE_SYSCALL(0xA044, SYS_A044);
DEFINE_SYSCALL(0xA048, SYS_A048);
DEFINE_SYSCALL(0xA04C, SYS_A04C);
DEFINE_SYSCALL(0xA050, SYS_A050);
DEFINE_SYSCALL(0xA054, SYS_A054);
DEFINE_SYSCALL(0xA058, SYS_A058);
DEFINE_SYSCALL(0xA05C, SYS_A05C);
DEFINE_SYSCALL(0xA060, SYS_A060);
DEFINE_SYSCALL(0xA064, SYS_A064);
DEFINE_SYSCALL(0xA068, SYS_A068);
DEFINE_SYSCALL(0xA06C, SYS_A06C);
DEFINE_SYSCALL(0xA070, SYS_A070);
DEFINE_SYSCALL(0xA074, SYS_A074);
DEFINE_SYSCALL(0xA078, SYS_A078);
DEFINE_SYSCALL(0xA07C, SYS_A07C);
DEFINE_SYSCALL(0xA080, SYS_A080);
DEFINE_SYSCALL(0xA084, SYS_A084);
DEFINE_SYSCALL(0xA088, SYS_A088);
DEFINE_SYSCALL(0xA08C, SYS_A08C);
DEFINE_SYSCALL(0xA090, SYS_A090);
DEFINE_SYSCALL(0xA094, SYS_A094);
DEFINE_SYSCALL(0xA098, SYS_A098);
DEFINE_SYSCALL(0xA09C, IsKeyReady);
DEFINE_SYSCALL(0xA0A0, SYS_A0A0);
DEFINE_SYSCALL(0xA0A4, SYS_A0A4);
DEFINE_SYSCALL(0xA0A8, SYS_A0A8);
DEFINE_SYSCALL(0xA0AC, SYS_A0AC);
DEFINE_SYSCALL(0xA0B0, SYS_A0B0);
DEFINE_SYSCALL(0xA0B4, SYS_A0B4);
DEFINE_SYSCALL(0xA0B8, SYS_A0B8);
DEFINE_SYSCALL(0xA0BC, SYS_A0BC);
DEFINE_SYSCALL(0xA0C0, SYS_A0C0);
DEFINE_SYSCALL(0xA0C4, SYS_A0C4);
DEFINE_SYSCALL(0xA0C8, SYS_A0C8);
DEFINE_SYSCALL(0xA0CC, SYS_A0CC);
DEFINE_SYSCALL(0xA0D0, SYS_A0D0);
DEFINE_SYSCALL(0xA0D4, SYS_A0D4);
DEFINE_SYSCALL(0xA0D8, SYS_A0D8);
DEFINE_SYSCALL(0xA0DC, SYS_A0DC);
DEFINE_SYSCALL(0xA0E0, SYS_A0E0);
DEFINE_SYSCALL(0xA0E4, SYS_A0E4);
DEFINE_SYSCALL(0xA0E8, SYS_A0E8);
DEFINE_SYSCALL(0xA0EC, SYS_A0EC);
DEFINE_SYSCALL(0xA0F0, SYS_A0F0);
DEFINE_SYSCALL(0xA0F4, SYS_A0F4);
DEFINE_SYSCALL(0xA0F8, SYS_A0F8);
DEFINE_SYSCALL(0xA0FC, SYS_A0FC);
DEFINE_SYSCALL(0xA100, SYS_A100);
DEFINE_SYSCALL(0xA104, SYS_A104);
DEFINE_SYSCALL(0xA108, SYS_A108);
DEFINE_SYSCALL(0xA10C, SYS_A10C);
DEFINE_SYSCALL(0xA110, SYS_A110);
DEFINE_SYSCALL(0xA114, SYS_A114);
DEFINE_SYSCALL(0xA118, SYS_A118);
DEFINE_SYSCALL(0xA11C, SYS_A11C);
DEFINE_SYSCALL(0xA120, SYS_A120);
DEFINE_SYSCALL(0xA124, SYS_A124);
DEFINE_SYSCALL(0xA128, SYS_A128);
DEFINE_SYSCALL(0xA12C, SYS_A12C);
DEFINE_SYSCALL(0xA130, SYS_A130);
DEFINE_SYSCALL(0xA134, SYS_A134);
DEFINE_SYSCALL(0xA138, SYS_A138);
DEFINE_SYSCALL(0xA13C, SYS_A13C);
DEFINE_SYSCALL(0xA140, SYS_A140);
DEFINE_SYSCALL(0xA144, SYS_A144);
DEFINE_SYSCALL(0xA148, SYS_A148);
DEFINE_SYSCALL(0xA14C, SYS_A14C);
DEFINE_SYSCALL(0xA150, SYS_A150);
DEFINE_SYSCALL(0xA154, SYS_A154);
DEFINE_SYSCALL(0xA158, SYS_A158);
DEFINE_SYSCALL(0xA15C, SYS_A15C);
DEFINE_SYSCALL(0xA160, SYS_A160);
DEFINE_SYSCALL(0xA164, SYS_A164);
DEFINE_SYSCALL(0xA168, SYS_A168);
DEFINE_SYSCALL(0xA16C, SYS_A16C);
DEFINE_SYSCALL(0xA170, SYS_A170);
DEFINE_SYSCALL(0xA174, SYS_A174);
DEFINE_SYSCALL(0xA178, SYS_A178);
DEFINE_SYSCALL(0xA17C, SYS_A17C);
DEFINE_SYSCALL(0xA180, SYS_A180);
DEFINE_SYSCALL(0xA184, SYS_A184);
DEFINE_SYSCALL(0xA188, SYS_A188);
DEFINE_SYSCALL(0xA18C, SYS_A18C);
DEFINE_SYSCALL(0xA190, SYS_A190);
DEFINE_SYSCALL(0xA194, SYS_A194);
DEFINE_SYSCALL(0xA198, SYS_A198);
DEFINE_SYSCALL(0xA19C, SYS_A19C);
DEFINE_SYSCALL(0xA1A0, SYS_A1A0);
DEFINE_SYSCALL(0xA1A4, SYS_A1A4);
DEFINE_SYSCALL(0xA1A8, SYS_A1A8);
DEFINE_SYSCALL(0xA1AC, SYS_A1AC);
DEFINE_SYSCALL(0xA1B0, SYS_A1B0);
DEFINE_SYSCALL(0xA1B4, SYS_A1B4);
DEFINE_SYSCALL(0xA1B8, SYS_A1B8);
DEFINE_SYSCALL(0xA1BC, SYS_A1BC);
DEFINE_SYSCALL(0xA1C0, SYS_A1C0);
DEFINE_SYSCALL(0xA1C4, SYS_A1C4);
DEFINE_SYSCALL(0xA1C8, SYS_A1C8);
DEFINE_SYSCALL(0xA1CC, SYS_A1CC);
DEFINE_SYSCALL(0xA1D0, SYS_A1D0);
DEFINE_SYSCALL(0xA1D4, SYS_A1D4);
DEFINE_SYSCALL(0xA1D8, SYS_A1D8);
DEFINE_SYSCALL(0xA1DC, SYS_A1DC);
DEFINE_SYSCALL(0xA1E0, SYS_A1E0);
DEFINE_SYSCALL(0xA1E4, SYS_A1E4);
DEFINE_SYSCALL(0xA1E8, SYS_A1E8);
DEFINE_SYSCALL(0xA1EC, SYS_A1EC);
DEFINE_SYSCALL(0xA1F0, SYS_A1F0);
DEFINE_SYSCALL(0xA1F4, SYS_A1F4);
DEFINE_SYSCALL(0xA1F8, SYS_A1F8);
DEFINE_SYSCALL(0xA1FC, SYS_A1FC);
DEFINE_SYSCALL(0xA200, SYS_A200);
DEFINE_SYSCALL(0xA204, SYS_A204);
DEFINE_SYSCALL(0xA208, SYS_A208);
DEFINE_SYSCALL(0xA20C, SYS_A20C);
DEFINE_SYSCALL(0xA210, SYS_A210);
DEFINE_SYSCALL(0xA214, SYS_A214);
DEFINE_SYSCALL(0xA218, SYS_A218);
DEFINE_SYSCALL(0xA21C, SYS_A21C);
DEFINE_SYSCALL(0xA220, SYS_A220);
DEFINE_SYSCALL(0xA224, SYS_A224);
DEFINE_SYSCALL(0xA228, SYS_A228);
DEFINE_SYSCALL(0xA22C, SYS_A22C);
DEFINE_SYSCALL(0xA230, SYS_A230);
DEFINE_SYSCALL(0xA234, SYS_A234);
DEFINE_SYSCALL(0xA238, SYS_A238);
DEFINE_SYSCALL(0xA23C, SYS_A23C);
DEFINE_SYSCALL(0xA240, SYS_A240);
DEFINE_SYSCALL(0xA244, SYS_A244);
DEFINE_SYSCALL(0xA248, SYS_A248);
DEFINE_SYSCALL(0xA24C, SYS_A24C);
DEFINE_SYSCALL(0xA250, SYS_A250);
DEFINE_SYSCALL(0xA254, SYS_A254);
DEFINE_SYSCALL(0xA258, SYS_A258);
DEFINE_SYSCALL(0xA25C, SYS_A25C);
DEFINE_SYSCALL(0xA260, SYS_A260);
DEFINE_SYSCALL(0xA264, SYS_A264);
DEFINE_SYSCALL(0xA268, SYS_A268);
DEFINE_SYSCALL(0xA26C, SYS_A26C);
DEFINE_SYSCALL(0xA270, SYS_A270);
DEFINE_SYSCALL(0xA274, SYS_A274);
DEFINE_SYSCALL(0xA278, SYS_A278);
DEFINE_SYSCALL(0xA27C, SYS_A27C);
DEFINE_SYSCALL(0xA280, SYS_A280);
DEFINE_SYSCALL(0xA284, SYS_A284);
DEFINE_SYSCALL(0xA288, SYS_A288);
DEFINE_SYSCALL(0xA28C, SYS_A28C);
DEFINE_SYSCALL(0xA290, SYS_A290);
DEFINE_SYSCALL(0xA294, SYS_A294);
DEFINE_SYSCALL(0xA298, SYS_A298);
DEFINE_SYSCALL(0xA29C, SYS_A29C);
DEFINE_SYSCALL(0xA2A0, SYS_A2A0);
DEFINE_SYSCALL(0xA2A4, SYS_A2A4);
DEFINE_SYSCALL(0xA2A8, SYS_A2A8);
DEFINE_SYSCALL(0xA2AC, SYS_A2AC);
DEFINE_SYSCALL(0xA2B0, SYS_A2B0);
DEFINE_SYSCALL(0xA2B4, SYS_A2B4);
DEFINE_SYSCALL(0xA2B8, SYS_A2B8);
DEFINE_SYSCALL(0xA2BC, SYS_A2BC);
DEFINE_SYSCALL(0xA2C0, SYS_A2C0);
DEFINE_SYSCALL(0xA2C4, SYS_A2C4);
DEFINE_SYSCALL(0xA2C8, SYS_A2C8);
DEFINE_SYSCALL(0xA2CC, SYS_A2CC);
DEFINE_SYSCALL(0xA2D0, SYS_A2D0);
DEFINE_SYSCALL(0xA2D4, SYS_A2D4);
DEFINE_SYSCALL(0xA2D8, SYS_A2D8);
DEFINE_SYSCALL(0xA2DC, SYS_A2DC);
DEFINE_SYSCALL(0xA2E0, SYS_A2E0);
DEFINE_SYSCALL(0xA2E4, SYS_A2E4);
DEFINE_SYSCALL(0xA2E8, SYS_A2E8);
DEFINE_SYSCALL(0xA2EC, SYS_A2EC);
DEFINE_SYSCALL(0xA2F0, SYS_A2F0);
DEFINE_SYSCALL(0xA2F4, SYS_A2F4);
DEFINE_SYSCALL(0xA2F8, SYS_A2F8);
DEFINE_SYSCALL(0xA2FC, SYS_A2FC);
DEFINE_SYSCALL(0xA300, SYS_A300);
DEFINE_SYSCALL(0xA304, SYS_A304);
DEFINE_SYSCALL(0xA308, SYS_A308);
DEFINE_SYSCALL(0xA30C, SYS_A30C);
DEFINE_SYSCALL(0xA310, SYS_A310);
DEFINE_SYSCALL(0xA314, SYS_A314);
DEFINE_SYSCALL(0xA318, SYS_A318);
DEFINE_SYSCALL(0xA31C, SYS_A31C);
DEFINE_SYSCALL(0xA320, SYS_A320);
DEFINE_SYSCALL(0xA324, SYS_A324);
DEFINE_SYSCALL(0xA328, SYS_A328);
DEFINE_SYSCALL(0xA32C, SYS_A32C);
DEFINE_SYSCALL(0xA330, SYS_A330);
DEFINE_SYSCALL(0xA334, SYS_A334);
DEFINE_SYSCALL(0xA338, abort);
DEFINE_SYSCALL(0xA33C, atoi);
DEFINE_SYSCALL(0xA340, atol);
DEFINE_SYSCALL(0xA344, SYS_A344);
DEFINE_SYSCALL(0xA348, fgetc);
DEFINE_SYSCALL(0xA34C, fprintf);
DEFINE_SYSCALL(0xA350, fputc);
DEFINE_SYSCALL(0xA354, fscanf);
DEFINE_SYSCALL(0xA358, memchr);
DEFINE_SYSCALL(0xA35C, memcmp);
DEFINE_SYSCALL(0xA360, memcpy);
DEFINE_SYSCALL(0xA364, memmove);
DEFINE_SYSCALL(0xA368, memset);
DEFINE_SYSCALL(0xA36C, printf);
DEFINE_SYSCALL(0xA370, rand);
DEFINE_SYSCALL(0xA374, scanf);
DEFINE_SYSCALL(0xA378, sprintf);
DEFINE_SYSCALL(0xA37C, srand);
DEFINE_SYSCALL(0xA380, strcat);
DEFINE_SYSCALL(0xA384, strchr);
DEFINE_SYSCALL(0xA388, strcmp);
DEFINE_SYSCALL(0xA38C, strcpy);
DEFINE_SYSCALL(0xA390, strlen);
DEFINE_SYSCALL(0xA394, strncat);
DEFINE_SYSCALL(0xA398, strncmp);
DEFINE_SYSCALL(0xA39C, strncpy);
DEFINE_SYSCALL(0xA3A0, strrchr);
DEFINE_SYSCALL(0xA3A4, strstr);
DEFINE_SYSCALL(0xA3A8, tolower);
DEFINE_SYSCALL(0xA3AC, toupper);
DEFINE_SYSCALL(0xA3B0, SYS_A3B0);
DEFINE_SYSCALL(0xA3B4, SYS_A3B4);
DEFINE_SYSCALL(0xA3B8, SYS_A3B8);
DEFINE_SYSCALL(0xA3BC, SYS_A3BC);
DEFINE_SYSCALL(0xA3C0, SYS_A3C0);
DEFINE_SYSCALL(0xA3C4, SYS_A3C4);
DEFINE_SYSCALL(0xA3C8, SYS_A3C8);
DEFINE_SYSCALL(0xA3CC, SYS_A3CC);
DEFINE_SYSCALL(0xA3D0, SYS_A3D0);
DEFINE_SYSCALL(0xA3D4, SYS_A3D4);
DEFINE_SYSCALL(0xA3D8, SYS_A3D8);
DEFINE_SYSCALL(0xA3DC, SYS_A3DC);
DEFINE_SYSCALL(0xA3E0, SYS_A3E0);
DEFINE_SYSCALL(0xA3E4, SYS_A3E4);
DEFINE_SYSCALL(0xA3E8, SYS_A3E8);
DEFINE_SYSCALL(0xA3EC, SYS_A3EC);
DEFINE_SYSCALL(0xA3F0, SYS_A3F0);
DEFINE_SYSCALL(0xA3F4, SYS_A3F4);
DEFINE_SYSCALL(0xA3F8, SYS_A3F8);
DEFINE_SYSCALL(0xA3FC, SYS_A3FC);
DEFINE_SYSCALL(0xA400, SYS_A400);
DEFINE_SYSCALL(0xA404, SYS_A404);
DEFINE_SYSCALL(0xA408, SYS_A408);
DEFINE_SYSCALL(0xA40C, SYS_A40C);
DEFINE_SYSCALL(0xA410, SYS_A410);
DEFINE_SYSCALL(0xA414, SYS_A414);
DEFINE_SYSCALL(0xA418, SYS_A418);
DEFINE_SYSCALL(0xA41C, SYS_A41C);
DEFINE_SYSCALL(0xA420, SYS_A420);
DEFINE_SYSCALL(0xA424, SYS_A424);
DEFINE_SYSCALL(0xA428, SYS_A428);
DEFINE_SYSCALL(0xA42C, SYS_A42C);
DEFINE_SYSCALL(0xA430, SYS_A430);
DEFINE_SYSCALL(0xA434, SYS_A434);
DEFINE_SYSCALL(0xA438, SYS_A438);
DEFINE_SYSCALL(0xA43C, SYS_A43C);
DEFINE_SYSCALL(0xA440, SYS_A440);
DEFINE_SYSCALL(0xA444, SYS_A444);
DEFINE_SYSCALL(0xA448, SYS_A448);
DEFINE_SYSCALL(0xA44C, SYS_A44C);
DEFINE_SYSCALL(0xA450, SYS_A450);
DEFINE_SYSCALL(0xA454, SYS_A454);
DEFINE_SYSCALL(0xA458, SYS_A458);
DEFINE_SYSCALL(0xA45C, SYS_A45C);
DEFINE_SYSCALL(0xA460, SYS_A460);
DEFINE_SYSCALL(0xA464, SYS_A464);
DEFINE_SYSCALL(0xA468, SYS_A468);
DEFINE_SYSCALL(0xA46C, SYS_A46C);
DEFINE_SYSCALL(0xA470, SYS_A470);

int fputs(const char* str, FILE* stream) {
    while(*str) {
        if(fputc(*str++, stream) == EOF) {
            return EOF;
        }
    }
    return fputc('\n', stream);
}

int getchar(void) {
    return fgetc(stdin);
}

int putchar(int character) {
    return fputc(character, stdout);
}

int puts(const char* str) {
    return fputs(str, stdout);
}
