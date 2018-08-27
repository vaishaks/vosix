static unsigned int LOCK = 0;

void try_lock (int cpuLock)
{
    while(LOCK != cpuLock);    
}

void close_lock()
{
    LOCK = (LOCK + 1) % 4;
}