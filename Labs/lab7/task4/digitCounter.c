int digit_cnt(char *arg)
{
    int counter = 0;
    while (*arg != '\0')
    {
        if (*arg >= '0' && *arg <= '9')
            counter++;
        arg++;
    }
    return counter;
}

int main(int argc, char **argv)
{
    int num = digit_cnt(argv[1]);
    return num;
}