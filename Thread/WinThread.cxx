#define ADD_EXPORTS

#include <WinThread.hxx>

#define DEFAULT 0

HANDLE Osal::Thread::createDefaultThread(ThreadProc threadProc, LPVOID lpParameter)
{
    LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
    SIZE_T dwStackSize = DEFAULT;
    LPTHREAD_START_ROUTINE lpStartAddress = threadProc;
    DWORD dwCreationFlags = DEFAULT;
    LPDWORD lpThreadId = NULL;

    HANDLE threadHdr = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);

    return threadHdr;
}