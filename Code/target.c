#include "target.h"
#include <string.h>
#include <assert.h>
extern FILE* output;
static const int max_stack_space = 20000;
static const int INIT_ARGOFF = 20;
static record_t* record = NULL;
static int now_off = -4;
static int argoff;
static record_t* new_r(char* name, int off) {
    record_t* t = malloc(sizeof(record_t));
    strcpy(t->name, name);
    t->off = off;
    return t;
}
static char* regu_imm(char* a) {
    char* res = malloc(20);
    for (int i = 1;; i++) {
        res[i - 1] = a[i];
        if (a[i] == '\0')
            break;
    }
    return res;
}
static void reserve_space(int byte) {
    fprintf(output, "  addi $sp,$sp,%d\n", byte);
}
static void save(char* regname,int off){
    fprintf(output, "  sw %s,%d($fp)\n", regname, off);
}
static void load(char* regname,int off){
    fprintf(output, "  lw %s,%d($fp)\n", regname, off);
}
static void push(char* regname){
    fprintf(output, "  addi $sp,-4\n");
    fprintf(output, "  sw %s,0($sp)\n", regname);
}
static void pop(char* regname){
    fprintf(output, "  lw %s,0($sp)\n", regname);
    fprintf(output, "  addi $sp 4\n");
}
static void pushall(){
    push("$t0");
    push("$t1");
    push("$t2");
    push("$ra");
    push("$fp");
}
static void popall(){
    pop("$fp");
    pop("$ra");
    pop("$t2");
    pop("$t1");
    pop("$t0");
}
static void add_record(record_t* r){
    if (record==NULL)
        record = r;
    else{
        r->next = record;
        record = r;
    }
}
static char* rt[] = {
    "$t0", "$t1", "$t2"};
static int get_reg(){
    static int now = 0;
    int ans = now;
    now = (now + 1) % 3;
    return ans;
}
static int get_off(char* name){
    for (record_t* head = record; head;head=head->next){
        if (strcmp(head->name,name)==0){
            return head->off;
        }
    }
    record_t* t = new_r(name, now_off);
    add_record(t);
    now_off -= 4;
    return t->off;
}
static void load_arg2reg(int reg, operand_t arg) {
    if (arg.kind == OPE_IMM) {
        fprintf(output, "  li %s,%s\n", rt[reg], regu_imm(arg.name));
    } else {
        int off = get_off(arg.name);
        load(rt[reg], off);
    }
}
static void gen_head(){
    fprintf(output, ".data\n_prompt: .asciiz \"Enter an integer:\"\n_ret: .asciiz \"\\n\"\n.globl main\n.text\n");
}
static void dec_write(){
    char* name = "write:\n  li $v0,1\n  syscall\n  li $v0,4\n  la $a0,_ret\n  syscall\n  move $v0,$0\n  jr $ra\n";
    fprintf(output, "%s",name);
}
static void dec_read(){
    char* name = "read:\n  li $v0,4\n  la $a0,_prompt\n  syscall\n  li $v0,5\n  syscall\n  jr $ra\n";
    fprintf(output, "%s",name);
}
void gen_target() {
    gen_head();
    dec_read();
    dec_write();
    for (int i = 0; i < clst.num; i++) {
        gen_one(clst.lst[i]);
    }
}
void gen_one(code_entry* ce){
    operand_kind opkind = ce->op.kind;
    switch (opkind){
        case OP_ASSIGN:{
            gen_assign(ce);
            break;
        }
        case OP_PLUS:{
            gen_plus(ce);
            break;
        }
        case OP_MINUS:{
            gen_minus(ce);
            break;
        }
        case OP_MUL:{
            gen_mul(ce);
            break;
        }
        case OP_DIV:{
            gen_div(ce);
            break;
        }
        case OP_STAR:{
            gen_star(ce);
            break;
        }
        case OP_TOSTAR:{
            gen_tostar(ce);
            break;
        }
        case OP_GOTO:{
            gen_goto(ce);
            break;
        }
        case OP_CALL:{
            gen_call(ce);
            break;
        }
        case OP_RETURN:{
            gen_return(ce);
            break;
        }
        case OP_IF_GOTO:{
            gen_if_goto(ce);
            break;
        }
        case OP_FUNCDEC:{
            gen_funcdec(ce);
            break;
        }
        case OP_LABELDEC:{
            gen_labeldec(ce);
            break;
        }
        case OP_WRITE:{
            gen_write(ce);
            break;
        }
        case OP_READ:{
            gen_read(ce);
            break;
        }
        case OP_ARG:{
            gen_arg(ce);
            break;
        }
        case OP_PARAM:{
            gen_param(ce);
            break;
        }
        case OP_DEC:{
            gen_dec(ce);
            break;
        }
        case OP_ADDR:{
            gen_addr(ce);
            break;
        }
            assert(0);
    }
}
void gen_assign(code_entry* ce){
    operand_kind kind2 = ce->arg2.kind;
    if (kind2==OPE_IMM){
        int reg1 = get_reg();
        int off = get_off(ce->arg1.name);
        fprintf(output, "  li %s,%s\n", rt[reg1], regu_imm(ce->arg2.name));
        save(rt[reg1], off);
    } else {
        int reg1 = get_reg();
        int reg2 = get_reg();
        int off1 = get_off(ce->arg1.name), off2 = get_off(ce->arg2.name);
        load(rt[reg2], off2);
        fprintf(output, "  move %s,%s\n", rt[reg1], rt[reg2]);
        save(rt[reg1], off1);
    }
}
void gen_plus(code_entry* ce){
    int r1 = get_reg(), r2 = get_reg();
    int off3 = get_off(ce->result.name);
    load_arg2reg(r1, ce->arg1);
    load_arg2reg(r2, ce->arg2);
    fprintf(output, "  add %s,%s,%s\n", rt[r1], rt[r1], rt[r2]);
    save(rt[r1], off3);
}
void gen_minus(code_entry* ce){
    int r1 = get_reg(), r2 = get_reg();
    int off3 = get_off(ce->result.name);
    load_arg2reg(r1, ce->arg1);
    load_arg2reg(r2, ce->arg2);
    fprintf(output, "  sub %s,%s,%s\n", rt[r1], rt[r1], rt[r2]);
    save(rt[r1], off3);
}
void gen_mul(code_entry* ce){
    int r1 = get_reg(), r2 = get_reg();
    int off3 = get_off(ce->result.name);
    load_arg2reg(r1, ce->arg1);
    load_arg2reg(r2, ce->arg2);
    fprintf(output, "  mul %s,%s,%s\n", rt[r1], rt[r1], rt[r2]);
    save(rt[r1], off3);
}
void gen_div(code_entry* ce){
    int r1 = get_reg(), r2 = get_reg();
    int off3 = get_off(ce->result.name);
    load_arg2reg(r1, ce->arg1);
    load_arg2reg(r2, ce->arg2);
    fprintf(output, "  div %s,%s\n", rt[r1], rt[r2]);
    int r3 = get_reg();
    fprintf(output, "  mflo %s\n", rt[r3]);
    save(rt[r3], off3);
}
void gen_star(code_entry* ce){
    int r1 = get_reg(), r2 = get_reg();
    load_arg2reg(r2, ce->arg1);
    fprintf(output, "  lw %s,0(%s)\n", rt[r1], rt[r2]);
    int off = get_off(ce->result.name);
    save(rt[r1], off);
}
void gen_tostar(code_entry* ce){
    int r1 = get_reg(), r2 = get_reg();
    load_arg2reg(r1, ce->result);
    load_arg2reg(r2, ce->arg1);
    fprintf(output, "  sw %s,0(%s)\n", rt[r2], rt[r1]);
}
void gen_goto(code_entry* ce){
    fprintf(output, "  j %s\n", ce->result.name);
}
void gen_return(code_entry* ce){
    int r = get_reg();
    load_arg2reg(r, ce->arg1);
    fprintf(output, "  move $v0,%s\n", rt[r]);
    fprintf(output, "  move $sp,$fp\n");
    fprintf(output, "  jr $ra\n");
}
void gen_call(code_entry* ce){
    pushall();
    fprintf(output, "  jal %s\n", ce->arg1.name);
    popall();
    int r = get_reg();
    int off = get_off(ce->result.name);
    fprintf(output, "  move %s,$v0\n", rt[r]);
    save(rt[r], off);
}
void gen_if_goto(code_entry* ce){
    int r1 = get_reg(), r2 = get_reg();
    load_arg2reg(r1, ce->arg1), load_arg2reg(r2, ce->arg2);
    if (strcmp(ce->gotoop.name, "==") == 0) {
        fprintf(output, "  beq %s,%s,%s\n", rt[r1], rt[r2], ce->result.name);
    } else if (strcmp(ce->gotoop.name, "!=") == 0) {
        fprintf(output, "  bne %s,%s,%s\n", rt[r1], rt[r2], ce->result.name);
    } else if (strcmp(ce->gotoop.name, ">") == 0) {
        fprintf(output, "  bgt %s,%s,%s\n", rt[r1], rt[r2], ce->result.name);
    } else if (strcmp(ce->gotoop.name, "<") == 0) {
        fprintf(output, "  blt %s,%s,%s\n", rt[r1], rt[r2], ce->result.name);
    } else if (strcmp(ce->gotoop.name, ">=") == 0) {
        fprintf(output, "  bge %s,%s,%s\n", rt[r1], rt[r2], ce->result.name);
    } else if (strcmp(ce->gotoop.name, "<=") == 0) {
        fprintf(output, "  ble %s,%s,%s\n", rt[r1], rt[r2], ce->result.name);
    }
}
void gen_funcdec(code_entry* ce){
    fprintf(output, "%s:\n", ce->arg1.name);
    fprintf(output, "  move $fp,$sp\n");
    reserve_space(-max_stack_space);
    now_off = -4;
    argoff = INIT_ARGOFF;
}
void gen_labeldec(code_entry* ce){
    fprintf(output, "%s:\n", ce->arg1.name);
}
void gen_write(code_entry* ce){
    int r = get_reg();
    load_arg2reg(r, ce->arg1);
    fprintf(output, "  move $a0,%s\n", rt[r]);
    pushall();
    fprintf(output, "  jal write\n");
    popall();
}
void gen_read(code_entry* ce){
    pushall();
    fprintf(output, "  jal read\n");
    popall();
    int r = get_reg();
    int off = get_off(ce->result.name);
    fprintf(output, "  move %s,$v0\n", rt[r]);
    save(rt[r], off);
}
void gen_param(code_entry* ce){
    record_t* t = new_r(ce->arg1.name, argoff);
    argoff += 4;
    add_record(t);
}
void gen_arg(code_entry* ce){
    int reg = get_reg();
    load_arg2reg(reg, ce->arg1);
    push(rt[reg]);
}
void gen_dec(code_entry* ce){
    int sz = atoi(ce->arg2.name);
    record_t* t = new_r(ce->arg1.name, now_off - sz + 4);
    now_off -= sz;
    add_record(t);
}
void gen_addr(code_entry* ce){
    int off = get_off(ce->arg1.name);
    int r1 = get_reg();
    fprintf(output, "  addi %s,$fp,%d\n", rt[r1], off);
    int off1 = get_off(ce->result.name);
    save(rt[r1], off1);
}