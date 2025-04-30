#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

SYSCALL_DEFINE2(reverse_string, const char __user *, input, char __user *, output) {
    char *kbuf_in, *kbuf_out;
    long len = 0, i;

    while (true) {
        char c;
        if (get_user(c, &input[len]))
            return -EFAULT;
        if (c == '\0')
            break;
        len++;
    }

    kbuf_in = kmalloc(len + 1, GFP_KERNEL);
    kbuf_out = kmalloc(len + 1, GFP_KERNEL);
    if (!kbuf_in || !kbuf_out)
        return -ENOMEM;

    if (copy_from_user(kbuf_in, input, len + 1)) {
        kfree(kbuf_in);
        kfree(kbuf_out);
        return -EFAULT;
    }

    for (i = 0; i < len; i++)
        kbuf_out[i] = kbuf_in[len - 1 - i];
    kbuf_out[len] = '\0';

    if (copy_to_user(output, kbuf_out, len + 1)) {
        kfree(kbuf_in);
        kfree(kbuf_out);
        return -EFAULT;
    }

    kfree(kbuf_in);
    kfree(kbuf_out);
    return 0;
}

