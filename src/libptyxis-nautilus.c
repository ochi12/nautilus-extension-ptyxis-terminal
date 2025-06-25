#include <gtk/gtk.h>
#include <nautilus-extension.h>
#include <libnautilus-extension/nautilus-menu-provider.h>

typedef struct {
    GObject parent_instance;
} NautilusExtensionPtyxisTerminal;

typedef struct {
    GObjectClass parent_class;
} NautilusExtensionPtyxisTerminalClass;


static void nautilus_extension_ptyxis_terminal_iface_init(NautilusMenuProviderInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (
    NautilusExtensionPtyxisTerminal,
    nautilus_extension_ptyxis_terminal,
    G_TYPE_OBJECT,
    0,
    G_IMPLEMENT_INTERFACE_DYNAMIC(NAUTILUS_TYPE_MENU_PROVIDER,
                                  nautilus_extension_ptyxis_terminal_iface_init)
);

void 
nautilus_module_initialize (GTypeModule *module)
{
  nautilus_extension_ptyxis_terminal_register_type(module);  
}

void
nautilus_module_shutdown(void) 
{
}

static void
open_terminal_cb(NautilusMenuItem *item, gpointer user_data) {
  char *path = (char *)user_data;
  char *sh_path = g_shell_quote(path);
  char *command = g_strdup_printf ("ptyxis --new-window --working-directory=%s", sh_path);
  g_spawn_command_line_async (command, NULL);
  g_free(command);
  g_free(sh_path);
}

static void
path_cleanup_cb(gpointer data, GClosure *closure) {
  g_free(data);
}

void
nautilus_module_list_types (const GType **types, int *n_types) {
  static GType type_list[1];

  type_list[0] = nautilus_extension_ptyxis_terminal_get_type();
  *types = type_list;
  *n_types = 1;
}

static GList *
nautilus_extension_ptyxis_terminal_get_file_items(NautilusMenuProvider *provider, 
                                                  GList *files) {
     
    if (!files || files->next != NULL) {
      return NULL;
    }
    
    NautilusFileInfo *file = files->data; 

    if (!file || !nautilus_file_info_is_directory(file)) {
      return NULL;
    }
    
    char *uri = nautilus_file_info_get_uri(file);
    GError *err = NULL;
    char *path = g_filename_from_uri(uri, NULL, &err);
   
    g_free(uri);
    if (err) {
      g_warning ("Failed to get filename from uri: %s", err->message);
      g_error_free(err);
      return NULL;
    }

    NautilusMenuItem *item = nautilus_menu_item_new(
        "NautilusExtensionPtyxisTerminal::open_terminal_from_file_item",
        "Open in Terminal",
        "Open the selected folder in Terminal",
        NULL
    );

    g_signal_connect_data(item, "activate", G_CALLBACK (open_terminal_cb), 
                          path, path_cleanup_cb, 0);

    return g_list_append(NULL, item);
}

static GList *
nautilus_extension_ptyxis_terminal_get_background_items(NautilusMenuProvider *provider,
                                                        NautilusFileInfo* current_folder)
{
    char *uri = nautilus_file_info_get_uri(current_folder);
    GError *err = NULL;
    char *path = g_filename_from_uri(uri, NULL, &err);
   
    g_free(uri);
    if (err) {
      g_warning ("Failed to get filename from uri: %s", err->message);
      g_error_free(err);
      return NULL;
    }

    NautilusMenuItem *item = nautilus_menu_item_new(
        "NautilusExtensionPtyxisTerminal::open_terminal_background_items",
        "Open in Terminal",
        "Open the selected folder in Terminal",
        NULL
    );

    g_signal_connect_data(item, "activate", G_CALLBACK (open_terminal_cb), 
                          path, path_cleanup_cb, 0);

    return g_list_append(NULL, item);

}

// Interface init: assign function to interface
static void
nautilus_extension_ptyxis_terminal_iface_init(NautilusMenuProviderInterface *iface) {
  iface->get_file_items = nautilus_extension_ptyxis_terminal_get_file_items;
  iface->get_background_items = nautilus_extension_ptyxis_terminal_get_background_items;
}

static void
nautilus_extension_ptyxis_terminal_init (NautilusExtensionPtyxisTerminal *self) {
  (void) self;
}

static void
nautilus_extension_ptyxis_terminal_class_init (NautilusExtensionPtyxisTerminalClass *klass) {
  (void) klass;
}

static void
nautilus_extension_ptyxis_terminal_class_finalize (NautilusExtensionPtyxisTerminalClass *klass) {
  (void) klass;
}

