#include <glib.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * Runs a command synchronously;
 */
gboolean run_command(const gchar *command)
{
    GError *error = NULL;
    gint status = 0;
    gchar *stdout_message = NULL;
    gchar *stderr_message = NULL;
    gboolean ret = FALSE;
    
    ret = g_spawn_command_line_sync(command, &stdout_message, &stderr_message, &status, &error);
    if ((error && (error->code != 0)) || ! WIFEXITED(status) || WEXITSTATUS(status))
        g_warning("Failed to execute command \"%s\", exited: %i, status: %i, stderr: %s, stdout: %s\n", command, WIFEXITED(status), WEXITSTATUS(status), stderr_message, stdout_message);
    else
        ret = TRUE;
    if (error)
        g_error_free(error);
    g_free(stdout_message);
    g_free(stderr_message);
    return ret;
}

gboolean check_for_ffmpeg()
{
    gchar *command = "ffmpeg -version";
    gboolean ret = run_command(command);
    if (! ret)
        g_critical("Could not find ffmpeg\n");
    return ret;
}

int main(int argc, char *argv[])
{
    gboolean has_ffmpeg = check_for_ffmpeg();
    if (! has_ffmpeg)
    {
        return 1;
    }
    else
    {
        g_print("Found ffmpeg\n");
    }
    return 0;
}

