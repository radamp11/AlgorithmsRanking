from django.shortcuts import render, get_object_or_404, redirect
from django.contrib.auth import authenticate, login, logout
from django.views.generic import View, ListView, DetailView, DeleteView
from django.views.generic.edit import CreateView
from django.db.models import Min
from .models import Algorithm, Outcome, Benchmark
from .forms import UserForm, LoginUserForm, CompareAlgorithms, ShowBenchmark
import mpld3  # TRZEBA ZAINSTALOWAC
import matplotlib.pyplot as plt
import matplotlib.style as mst

import sys

sys.path.append('../FileManagement/')

from FileManagement import szkielet


class DetailView(DetailView):
    model = Algorithm
    template_name = 'algorithms/detail.html'

    def get_context_data(self, **kwargs):
        alg = super().get_object()
        context = super().get_context_data(**kwargs)
        err_message = None
        if not alg.counted:
            try:
                error = szkielet.generateResults("media/" + str(alg.alg_file))
            except RuntimeError as ex:
                err_message = ex.args
            context['err_message'] = err_message

        if err_message != None:
            alg.delete()
        return context

    def get_object(self):
        alg = super().get_object()

        if not alg.counted:
            try:
                dims = szkielet.generateResults("media/" + str(alg.alg_file))
            except RuntimeError as ex:
                #message = ex.args 
                #print('wywaliło wyjatek')
                return alg
            if dims.__len__() == 0:
                alg.delete()
                return alg
            rownum = 1
            for dim in dims:
                with open('static/result_files/results' + str(dim) + '.txt', 'r') as myfile:
                    rownum = 1
                    for line in myfile:
                        out = Outcome()
                        out.algorithm = alg
                        out.dimension = dim
                        out.function = rownum
                        word_number = 1
                        for word in line.split():
                            if word_number == 1:
                                out.best = word
                            elif word_number == 2:
                                out.worst = word
                            elif word_number == 3:
                                out.median = word
                            elif word_number == 4:
                                out.mean_str = word
                                out.mean_float = float(word)
                                if dim == 10:
                                    alg.SE = alg.SE + 0.1 * out.mean_float
                                elif dim == 30:
                                    alg.SE = alg.SE + 0.2 * out.mean_float
                                elif dim == 50:
                                    alg.SE = alg.SE + 0.3 * out.mean_float
                                elif dim == 100:
                                    alg.SE = alg.SE + 0.4 * out.mean_float
                            else:
                                out.std = word

                            word_number = word_number + 1

                        rownum = rownum + 1
                        out.save()

            alg.num_of_fun = rownum - 1
            alg.counted = True
            alg.save()
        return alg


class AlgorithmListView(ListView):
    model = Algorithm

    def get_queryset(self):
        return Algorithm.objects.all()


def home(request):
    return render(request, 'algorithms/home.html')


class UserFormView(View):
    form_class = UserForm
    template_name = 'algorithms/registration.html'

    # wyswietla formularz
    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, {'form': form})

    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)

        if form.is_valid():
            user = form.save(commit=False)
            username = form.cleaned_data['username']
            password = form.cleaned_data['password']
            user.set_password(password)
            user.is_active = False
            user.save()

            if user is not None:
                return redirect('algorithms:wait')

        return render(request, self.template_name, {'form': form})


class LoginView(View):
    form_class = LoginUserForm
    template_name = 'algorithms/login.html'

    # wyswietla formularz
    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, {'form': form})

    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)

        username = request.POST['username']
        password = request.POST['password']

        user = authenticate(request, username=username, password=password)
        if user is not None:
            if user.is_active:
                login(request, user)
                return redirect('algorithms:home')
            else:
                return render(request, 'algorithms/wait-for-activate.html')

        return render(request, self.template_name, {'form': form})


def wait(request):
    return render(request, 'algorithms/wait-for-activate.html')


class AddAlgorithm(CreateView):
    model = Algorithm
    fields = ['name', 'author', 'alg_file']


# class DeleteAlgorithm(DeleteView):
#    model = Algorithm
#    success_url = reverse_lazy('algorithms:delete')

def delete(request):
    return render(request, 'algorithms/delete.html')


def logoutView(request):
    logout(request)
    return render(request, 'algorithms/logout.html')


class CompAlgView(View):
    form_class = CompareAlgorithms
    template_name = 'algorithms/compare.html'

    # wyswietla formularz
    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, {'form': form})

    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)
        if form.is_valid():
            alg1 = Algorithm.objects.get(pk=request.POST['algorithm1'])
            alg2 = Algorithm.objects.get(pk=request.POST['algorithm2'])

            fun = int(request.POST['function'])
            dim = int(request.POST['dimension'])

            file_name1 = str(alg1.alg_file)
            file_name2 = str(alg2.alg_file)

            plt.clf()
            mst.use('seaborn')
            fig = plt.figure(dpi=175)

            for x in [1,2]:
                try:
                    if x == 1:
                        mean_val = szkielet.calculateGraph("media/" + file_name1, fun, dim, 0)
                    else:
                        mean_val = szkielet.calculateGraph("media/" + file_name2, fun, dim, 0)
                    
                except RuntimeError as ex:
                    return render(request, self.template_name, { 'form': form, 'error_cpp' : ex.args })

                # x_axis = [0.01, 0.02, 0.03, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]
                x_axis = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]
                y_axis = mean_val
                while len(x_axis) > len(y_axis):
                    x_axis.pop()

                if x == 1:
                    plt.scatter(x_axis, y_axis, c='darkred', marker='o')
                    plt.plot(x_axis, y_axis, c='black', label="error value of   " + alg1.name)
                else:
                    plt.scatter(x_axis, y_axis, c='darkgreen', marker='o')
                    plt.plot(x_axis, y_axis, c='darkviolet', label="error value of   " + alg2.name)

            plt.title( "dim: " + str(dim) )
            plt.xlabel("FES")
            plt.ylabel("Mean Error Value")
            plt.ylim(bottom=-10)
            plt.grid(True)
            plt.legend()
            fig = plt.gcf()
            fig_html = mpld3.fig_to_html(fig)

            plt.clf()
            try:
                data_alg_1 = szkielet.calculateGraph("media/" + file_name1, fun, dim, 1)
                data_alg_2 = szkielet.calculateGraph("media/" + file_name2, fun, dim, 1)
            except RuntimeError as ex:
                return render(request, self.template_name, { 'form': form, 'error_cpp' : ex.args })
            
            data_boxplot = [data_alg_1, data_alg_2]
            plt.title("Boxplot")
            plt.boxplot(data_boxplot, positions=[0.75, 1.25], labels=[alg1.name, alg2.name])
            fig = plt.gcf()
            box_html = mpld3.fig_to_html(fig)

            function = Benchmark.objects.get(number=fun)
            fun_name = function.name
            fun_file_path = function.data_file_path

            out1 = Outcome.objects.get(algorithm=alg1.pk, dimension=int(dim), function=fun)
            out2 = Outcome.objects.get(algorithm=alg2.pk, dimension=int(dim), function=fun)
            return render(request, self.template_name,
                          {'form': form, 'fig_html': fig_html, 'box_html': box_html, 'out1': out1, 'out2': out2,
                           'fun_name': fun_name, 'fun_file_path': fun_file_path})
        else:
            return render(request, self.template_name, {'form': form})


class RankingView(ListView):
    model = Algorithm
    template_name = 'algorithms/ranking.html'

    def get_queryset(self):

        min_SE_2017 = Algorithm.objects.filter(num_of_fun = 30).aggregate(Min('SE'))
        min_SE_2013 = Algorithm.objects.filter(num_of_fun = 28).aggregate(Min('SE'))
        print(min_SE_2017['SE__min'])
        print(min_SE_2013['SE__min'])
        for alg in Algorithm.objects.all():
            if alg.num_of_fun == 30:
                alg.score1 = ((1 - (alg.SE - min_SE_2017['SE__min']) / (alg.SE)) * 50) 
            elif alg.num_of_fun == 28:
                alg.score1 = ((1 - (alg.SE - min_SE_2013['SE__min']) / (alg.SE)) * 50)
            alg.total_score = alg.score1 + alg.score2
            alg.save()
        return Algorithm.objects.order_by('-total_score')

        # 123zpr123


class BenchmarksView(View):
    form_class = ShowBenchmark
    template_name = 'algorithms/benchmarks_form.html'

    def get(self, request):
        form = self.form_class(None)
        return render(request, self.template_name, {'form': form})

    # przetwarzanie wpisanych danych
    def post(self, request):
        form = self.form_class(request.POST)
        if form.is_valid():
            bench = Benchmark.objects.get(pk=request.POST['benchmark'])
            name = bench.name
            file_path = bench.data_file_path

        return render(request, self.template_name, { 'form' : form, 'name': name, 'file_path': file_path })
