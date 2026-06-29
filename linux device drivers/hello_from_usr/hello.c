#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MADHU");
MODULE_DESCRIPTION("FIRST LKM");

static struct proc_dir_entry *custom_pointer;



static ssize_t read_procFile(struct file *fileptr, /*the pointer with which we will be moving in the buffer*/
                             char __user *user_space_buffer, /*the buffer into which the message will be read , which can be viewed in the user space*/
                             size_t count,/* choosen by the user , how many bites to be read*/
                             loff_t *offset /*number of bites to be read at a single streach*/)
{
    char msg[] = "hellow from kernel \n"; //mock of the kernel buffer that is going to be read
    size_t len = strlen(msg);
    if(*offset >= len*3) /*this makes the msg to be read and printed three times*/
    {
        printk("read once successfully\n");
        return 0;
    }

    *offset += len;
    if( copy_to_user(user_space_buffer , msg , len))
    {
        return -EFAULT;
    }
    return len;

}

static ssize_t write_procfile(struct file *FILE, const char __user *user_buff , size_t count, loff_t *offdet )
{
    char kernbuf[2048];
    ssize_t len = min(count , sizeof(kernbuf)-1) ;


 
    if(copy_from_user(kernbuf , user_buff , len))
    {
        return -EFAULT;
    }

    kernbuf[len] = '\0';

    return count;
}

static struct proc_ops proc_ops = {
    .proc_read = read_procFile , /* one of the structure membersz which is a function pointer , so we write an actual 
                                  function with the same signature and 
                                  equate it to the function pointer from the structure*/
                                  
    .proc_write = write_procfile                            
   
};




/*


THT INIT FUNCTION DEFINISION

*/

static int init_module_LKM(void)
{
    printk(KERN_INFO "hellow world from MADHU \n ");

   custom_pointer = proc_create("madhu", 0 , 
                                                 NULL,
                                                  &proc_ops);

    return 0;
}

/*

THE EXIT FUNCTION DEFINISION

*/

static void exit_module(void)
{
    printk(KERN_INFO "bye bye from MADHU\n");
    proc_remove(custom_pointer);
}


module_init(init_module_LKM);;
module_exit(exit_module);