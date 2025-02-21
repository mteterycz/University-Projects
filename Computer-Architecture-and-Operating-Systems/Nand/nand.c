#include "nand.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

// Definicja bramki NAND
typedef struct nand {
    unsigned n;
    void **inputs;
    int *input_types; // 0 - nic; 1 - bramka; 2 - sygnał;
    nand_t **outputs;
    int number_of_outputs;
    int outputs_size;
    int is_visited;
    bool output_value;
    ssize_t critical_path;
} nand_t;

// Nowa bramka NAND o n wejściach
nand_t* nand_new(unsigned n) {
    nand_t *new_nand = (nand_t*)malloc(sizeof(nand_t));
    // Błąd alokowania pamięci dla bramki
    if (new_nand == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    
    new_nand->n = n;
    
    new_nand->inputs = (void**)malloc(n * sizeof(void *));
    // Błąd alokowania pamięci dla tablicy wejść
    if (new_nand->inputs == NULL) {
        free(new_nand);
        errno = ENOMEM;
        return NULL;
    }
    
    new_nand->input_types = (int *)malloc(n * sizeof(int));
    // Błąd alokowania pamięci dla tablicy typów wejść
    if (new_nand->input_types == NULL) {
        free(new_nand->inputs);
        free(new_nand);
        errno = ENOMEM;
        return NULL;
    }
    for (unsigned i = 0; i < new_nand->n; ++i) new_nand->input_types[i] = 0;
    
    new_nand->outputs = (nand_t **)malloc(2 * sizeof(nand_t *));
    // Błąd alokowania pamięci dla tablicy wyjść
    if (new_nand->outputs == NULL) {
        free(new_nand->input_types);
        free(new_nand->inputs);
        free(new_nand);
        errno = ENOMEM;
        return NULL;
    }
    
    new_nand->number_of_outputs = 0;
    new_nand->outputs_size = 2;
    new_nand->is_visited = 0;
    new_nand->critical_path = -1;
    
    return new_nand;
}

//  Usunięcie bramki g
void nand_delete(nand_t *g) {
    if (g != NULL) {
        // Odłaczanie wejść
        for (unsigned i = 0; i < g->n; i++) {
            if (g->input_types[i] == 1) {
                nand_t *temp = (nand_t *)g->inputs[i];
                if (temp != NULL) {
                    for (int j = 0; j < temp->number_of_outputs; ++j) {
                        if (temp->outputs[j] == g) {
                            temp->outputs[j] = NULL;
                        }
                    }
                }
            }
        }
        
        // Odłączanie wyjść
        for (int i = 0; i < g->number_of_outputs; i++) {
            if (g->outputs[i] != NULL) {
                nand_t *temp = g->outputs[i];
                for (unsigned j = 0; j < temp->n; j++) {
                    if (temp->input_types[j] == 1 &&
                        (nand_t *)temp->inputs[j] == g) {
                        temp->input_types[j] = 0;
                    }
                }
            }
        }
        // Zwalnianie pamięci
        if (g->inputs != NULL) free(g->inputs);
        if (g->input_types != NULL) free(g->input_types);
        if (g->outputs != NULL) free(g->outputs);
        free(g);
    }
}

// Podłączenie wyjścia bramki g_out do wejścia k bramki g_in
int nand_connect_nand(nand_t *g_out, nand_t *g_in, unsigned k) {
    // Sprawdzenie poprawności argumentów
    if (g_out == NULL || g_in == NULL || k >= g_in->n) {
        errno = EINVAL;
        return -1;
    }
    // Zaalokowanie większej pamięci (jeśli potrzeba)
    if (g_out->number_of_outputs >= g_out->outputs_size) {
        g_out->outputs_size *= 2;
        nand_t **temp =g_out->outputs;
        g_out->outputs = realloc(g_out->outputs, 
                                 g_out->outputs_size * sizeof(nand_t *));
        if (g_out->outputs == NULL) {
            g_out->outputs = temp;
            g_out->outputs_size /= 2;
            //free(temp);
            errno = ENOMEM;
            return -1;
        }
        
    }
    // Odłączanie starego wejścia (bramki)
    if (g_in->input_types[k] == 1) {
        nand_t *temp = (nand_t *)g_in->inputs[k];
        if (temp->outputs != NULL) {
            int i = 0;
            while (temp->outputs[i] != g_in && i < temp->number_of_outputs) {
                ++i;
            }
            if (i < temp->number_of_outputs && temp->outputs[i] == g_in) {
                temp->outputs[i] = NULL;
            }
            g_in->input_types[k] = 0;
        }
    }
    // Podłączanie nowego wejścia
    g_in->inputs[k] = (nand_t *)g_out;
    g_in->input_types[k] = 1;
    
    g_out->outputs[g_out->number_of_outputs] = g_in;
    (g_out->number_of_outputs)++;
    return 0;
}

// Podłączenie sygnału boolowskiego s do wejścia k bramki g
int nand_connect_signal(bool const *s, nand_t *g, unsigned k) {
    // Sprawdzenie poprawności argumentów
    if (s == NULL || g == NULL || k >= g->n) {
        errno = EINVAL;
        return -1;
    }
    // Odłączanie starego wejścia (bramki)
    if (g->input_types[k] == 1) {
        nand_t *temp = (nand_t *)g->inputs[k];
        if (temp->outputs != NULL) {
            int i = 0;
            while ((nand_t *)temp->outputs[i] != g &&
                   i < temp->number_of_outputs) {
                ++i;
            }
            if (i < temp->number_of_outputs && temp->outputs[i] == g) {
                temp->outputs[i] = NULL;
            }
            g->input_types[k] = 0;
        }
    }
    // Podłączenie sygnału
    g->input_types[k] = 2;
    g->inputs[k] = (bool *)s;
    return 0;
}

// Wyznaczenie sygnału wyjściowego
bool output_signal(nand_t *g, bool *input_without_signal){
    if (!(*input_without_signal)) {
        if (g->is_visited == 1) {
            return g->output_value;
        }
        else {
            if (g->n == 0) {
                g->is_visited = 1;
                g->output_value = false;
                return false;
            }
            bool result = true;
            for (ssize_t i = 0; i < g->n; ++i) {
                if (g->input_types[i] == 2) {
                    result &= *((bool *)g->inputs[i]);
                }
                else if (g->input_types[i] == 1) {
                    result &= output_signal((nand_t *)g->inputs[i], 
                                            input_without_signal);
                }
                else {
                    *input_without_signal = true;
                }
            }
            g->output_value = !result;
            g->is_visited = 1;
            return !result; // NAND - negacja AND
        }
    }
    else {
        return false;
    }
}

// Sprawdzanie czy wystepuje cykl
void is_cycle(nand_t *g, bool *cycle) {
    if (g->n == 0 && !(*cycle)) return;
    else {
        if (g->is_visited == 1) *cycle = true;
        else {
            for (ssize_t i = 0; i < g->n; ++i) {
                if (!(*cycle)) {
                    g->is_visited = 1;
                    if (g->input_types[i] == 1) {
                        is_cycle((nand_t *)g->inputs[i], cycle);
                    }
                    g->is_visited = 0;
                }
            }
        }
    }
}

// Przywrócenie domyślnych wartości is_visited i critical_path
void is_visited_and_critical_path_to_default (nand_t *g) {
    if (g->is_visited == 0) return;
    else {
        for (ssize_t i = 0; i < g->n; ++i) {
            if (g->input_types[i] == 1) {
                is_visited_and_critical_path_to_default((nand_t *)g->inputs[i]);
            }
        }
        g->is_visited = 0;
        g->critical_path = -1;
    }
}

// Obliczanie najdłużej ścieżki dla bramki g
ssize_t calculate_max_path(nand_t *g) {
    if (g->critical_path > -1) {
        return g->critical_path;
    }
    if (g->n == 0){
        g->critical_path = 0;
        return 0;
    }
    
    ssize_t j = 0;
    while (j < g->n && g->input_types[j] == 2) {
        ++j;
    }
    if (j == g->n) {
        g->critical_path = 1;
        return 1;
    }
    
    ssize_t max_path = 0;
    for (ssize_t i = 0; i < g->n; ++i) {
        if (g->input_types[i] == 1) {
            ssize_t act_path = 1;
            act_path += calculate_max_path((nand_t *)g->inputs[i]);
            if (act_path > max_path) max_path = act_path;
        }
    }
    g->critical_path = max_path;
    return max_path;
        
}

// Wyznaczenie wartości sygnałów na wyjściach podanych bramek do tablicy s
// i zwrócenie długość ścieżki krytycznej
ssize_t nand_evaluate(nand_t **g, bool *s, size_t m) {
    // Sprawdzenie poprawności argumentów
    if (g == NULL || s == NULL || m == 0) {
        errno = EINVAL;
        return -1;
    }
    size_t p = 0;
    while (p < m && g[p] != NULL) ++p;
    if (p != m) {
        errno = EINVAL;
        return -1;
    }
    
    // Sprawdzanie czy jest cykl
    bool cycle = false;
    for (size_t i = 0; i < m; ++i) {
        is_cycle(g[i], &cycle);
    }
    if (cycle) {
        errno = ECANCELED;
        return -1;
    }
    
    // Przywrócenie domyślnych wartości is_visited i critical_path
    for (size_t i = 0; i < m; ++i) {
        is_visited_and_critical_path_to_default(g[i]);
    }
    
    // Wyznaczanie wyjść bramek i sprawdzanie poprawności wejść
    bool input_without_signal = false;
    for (size_t i = 0; i < m; ++i) {
        if (g[i]->is_visited == 1) {
            s[i] = g[i]->output_value;
        }
        else {
            s[i] = output_signal(g[i], &input_without_signal);
        }
        if (input_without_signal){
            errno = ECANCELED;
            return -1;
        }
    }
    
    // Wyznaczanie krytycznej ścieżki
    ssize_t max_lenght = -1;
    ssize_t max_path_of_g = -1;
    for (size_t i = 0; i < m; ++i) {
        max_path_of_g = calculate_max_path(g[i]);
        if (max_path_of_g > max_lenght) max_lenght = max_path_of_g;
    }
    
    // Przywrócenie domyślnych wartości is_visited i critical_path
    for (size_t i = 0; i < m; ++i) {
        is_visited_and_critical_path_to_default(g[i]);
    }
    
    return max_lenght;
}

// Wyznaczenie liczby wejść bramek podłączonych do wyjścia bramki g
ssize_t nand_fan_out(nand_t const *g) {
    // Sprawdzenie poprawności argumentów
    if (g == NULL) {
        errno = EINVAL;
        return -1;
    }
    ssize_t count = 0;
    for (int i = 0; i < g->number_of_outputs; ++i) {
        if (g->outputs[i] != NULL) ++count;
    }

    return count;
}

// Wskaźnik na bramke/sygnał podłączony do k-tego wejścia bramki g
void* nand_input(nand_t const *g, unsigned k) {
    // Sprawdzenie poprawności argumentów
    if (g == NULL || k >= g->n) {
        errno = EINVAL;
        return NULL;
    }
    if (g->input_types[k] == 0) {
        errno = 0;
        return NULL;
    }
    if (g->input_types[k] == 1) {
        return (nand_t *)g->inputs[k];
    }
    else {
        return (bool *)g->inputs[k];
    }
}

// Iterowanie po bramkach podłączonych do wyjścia bramki g
nand_t* nand_output(nand_t const *g, ssize_t k) {
    // Sprawdzenie poprawności argumentów
    if (g == NULL || k < 0 || k > nand_fan_out(g)) {
        errno = EINVAL;
        return NULL;
    }
    ssize_t count = -1;
    int i = 0;
    while (count < k) {
        if (g->outputs[i] != NULL) count++;
        ++i;
    }
    return g->outputs[i - 1];
}


